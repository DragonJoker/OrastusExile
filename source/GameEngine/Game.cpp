#include "GameEngine/Game.hpp"

#include "GameEngine/Ecs/AnimationData.hpp"
#include "GameEngine/Ecs/AttackData.hpp"
#include "GameEngine/ECS/WalkData.hpp"
#include "GameEngine/State/EnemyState.hpp"
#include "GameEngine/State/StateMachine.hpp"
#include "GameEngine/State/TowerState.hpp"

#include <Castor3D/Engine.hpp>
#include <Castor3D/Event/Frame/FrameListener.hpp>
#include <Castor3D/Event/Frame/CpuFunctorEvent.hpp>
#include <Castor3D/Material/Material.hpp>
#include <Castor3D/Model/Mesh/Mesh.hpp>
#include <Castor3D/Model/Mesh/Submesh/Submesh.hpp>
#include <Castor3D/Model/Skeleton/Skeleton.hpp>
#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/Scene.hpp>
#include <Castor3D/Scene/Animation/AnimatedObject.hpp>
#include <Castor3D/Scene/Animation/AnimatedObjectGroup.hpp>
#include <Castor3D/Scene/Light/Light.hpp>
#include <Castor3D/Scene/Light/PointLight.hpp>

using namespace castor;
using namespace castor3d;

namespace orastus
{
	//*********************************************************************************************

	namespace
	{
		void doPrepareGridLine( GridPathNode const & prv
			, GridPathNode const & cur
			, Grid & grid )
		{
			if ( prv.x != cur.x )
			{
				for ( auto x = std::min( prv.x, cur.x ); x < std::max( prv.x, cur.x ); ++x )
				{
					grid( prv.y - 1, x - 1 ).state = GridCell::State::ePath;
					grid( prv.y - 1, x - 0 ).state = GridCell::State::ePath;
					grid( prv.y - 0, x - 1 ).state = GridCell::State::ePath;
					grid( prv.y - 0, x - 0 ).state = GridCell::State::ePath;
				}
			}
			else
			{
				for ( auto y = std::min( prv.y, cur.y ); y <= std::max( prv.y, cur.y ); ++y )
				{
					grid( y - 1, prv.x - 1 ).state = GridCell::State::ePath;
					grid( y - 1, prv.x - 0 ).state = GridCell::State::ePath;
					grid( y - 0, prv.x - 1 ).state = GridCell::State::ePath;
					grid( y - 0, prv.x - 0 ).state = GridCell::State::ePath;
				}
			}
		}

		void doPrepareTarget( GridPathNode const & cur
			, Scene & scene
			, Grid & grid )
		{
			for ( uint32_t x = cur.x - 2; x <= cur.x + 2; ++x )
			{
				for ( uint32_t y = cur.y - 1; y <= cur.y + 3; ++y )
				{
					grid( y, x ).state = GridCell::State::ePath;
				}
			}

			grid( cur.y, cur.x ).state = GridCell::State::eTarget;
		}
	}

	//*********************************************************************************************

	void Game::SelectedEntity::select( Entity const & pentity
		, castor3d::GeometrySPtr pgeometry )
	{
		unselect();

		if ( pentity && pgeometry )
		{
			entity = pentity;
			geometry = pgeometry;
			materials.clear();
			auto & cache = geometry->getScene()->getEngine()->getMaterialCache();

			for ( auto submesh : *geometry->getMesh().lock() )
			{
				auto oldMat = geometry->getMaterial( *submesh );
				auto newMat = cache.find( oldMat->getName() + cuT( "_sel" ) ).lock();
				CU_Require( newMat );
				geometry->setMaterial( *submesh, newMat.get() );
				materials.push_back( oldMat );
			}
		}
	}

	void Game::SelectedEntity::unselect()
	{
		if ( entity && geometry )
		{
			uint32_t index{ 0u };
			auto mesh = geometry->getMesh().lock();

			for ( auto material : materials )
			{
				geometry->setMaterial( *mesh->getSubmesh( index++ ), material );
			}

			materials.clear();
			geometry.reset();
			entity = Entity{};
		}
	}

	//*********************************************************************************************

	Game::Game( castor3d::Scene & scene )
		: m_scene{ scene }
		, m_hud{ *this, scene }
		, m_path
		{
			{ 19,  1 },
			{ 19,  3 },
			{ 11,  3 },
			{ 11,  7 },
			{ 23,  7 },
			{ 23, 11 },
			{ 13, 11 },
			{ 13, 15 },
			{ 25, 15 },
			{ 25, 19 },
			{ 15, 19 },
			{ 15, 23 },
			{ 17, 23 },
			{ 17, 27 },
		}
		, m_enemySpawner{ m_ecs, *this }
		, m_bulletSpawner{ m_ecs, *this }
	{
		m_mapNode = m_scene.getSceneNodeCache().find( cuT( "MapBase" ) ).lock();
		m_emptyTileMesh = m_scene.getMeshCache().find( cuT( "EmptyTile" ) );
		m_pathTileMesh = m_scene.getMeshCache().find( cuT( "PathTile" ) );
		m_shortRangeTowerMesh = m_scene.getMeshCache().find( cuT( "ShortRange" ) ).lock();
		m_longRangeTowerMesh = m_scene.getMeshCache().find( cuT( "HeavySplash" ) ).lock();
		m_bulletMesh = m_scene.getMeshCache().find( cuT( "Bullet" ) ).lock();
		m_bulletMaterial = m_scene.getMaterialView().find( cuT( "Bullet" ) ).lock();
		m_targetNode = m_scene.getSceneNodeCache().find( cuT( "Target" ) ).lock();
		auto mesh = m_emptyTileMesh.lock();
		m_cellDimensions[0] = mesh->getBoundingBox().getMax()[0] - mesh->getBoundingBox().getMin()[0];
		m_cellDimensions[1] = mesh->getBoundingBox().getMax()[1] - mesh->getBoundingBox().getMin()[1];
		m_cellDimensions[2] = mesh->getBoundingBox().getMax()[2] - mesh->getBoundingBox().getMin()[2];
		m_hud.initialise();
		m_state = State::eInitial;
	}

	void Game::reset()
	{
		Grid grid;
		std::swap( m_grid, grid );
		m_state = State::eStarted;
		m_bulletSpawner.reset();
		m_enemySpawner.reset();
	}

	void Game::start()
	{
		doPrepareGrid();

		for ( auto & cell : m_grid )
		{
			switch ( cell.state )
			{
			case GridCell::State::eEmpty:
				doAddEmptyTile( cell );
				break;
			case GridCell::State::eStart:
			case GridCell::State::ePath:
				doAddPathTile( cell );
				break;
			default:
				break;
			}
		}

		auto & node = *m_path.rbegin();
		doAddTarget( m_grid( node.y, node.x ) );

		m_state = State::eStarted;
		m_hud.start();
		m_saved = Clock::now();
	}

	void Game::pause()
	{
		m_state = State::ePaused;
		m_hud.pause();
	}

	void Game::resume()
	{
		m_state = State::eStarted;
		m_hud.resume();
		m_saved = Clock::now();
	}

	void Game::help()
	{
		m_state = State::ePaused;
		m_hud.help();
	}

	void Game::update()
	{
		if ( isRunning() )
		{
#if !defined( NDEBUG )
			m_elapsed = std::chrono::milliseconds{ 40 };
#else
			m_elapsed = std::chrono::duration_cast< std::chrono::milliseconds >( Clock::now() - m_saved );
#endif
			m_ecs.update( *this, m_elapsed );
			m_enemySpawner.update( m_elapsed );
			m_saved = Clock::now();
		}
	}

	Entity Game::select( castor3d::GeometrySPtr geometry )
	{
		Entity result;
		auto entity = m_ecs.findEntity( m_ecs.getComponent( Ecs::GeometryComponent ), geometry );
		m_selectedCell = &doFindCell( entity );
		entity = m_selectedCell->entity;

		if ( m_ecs.hasComponent( entity, m_ecs.getComponent( Ecs::PickableComponent ) )
			&& m_ecs.getComponentData< bool >( entity, m_ecs.getComponent( Ecs::PickableComponent ) ).getValue() )
		{
			if ( doSelectEntity( entity ) )
			{
				result = entity;
			}
		}
		else
		{
			unselect();
		}

		return result;
	}

	void Game::unselect()
	{
		m_hud.hideBuild();
		m_hud.updateTowerInfo( m_ecs, Entity{} );
		m_selectedCell = nullptr;

		m_scene.getEngine()->postEvent( makeCpuFunctorEvent( EventType::ePreRender
			, [this]()
			{
				m_selectedBlock.unselect();
				m_selectedEntity.unselect();
			} ) );
	}

	void Game::killEnemy( Entity entity )
	{
		m_enemySpawner.killEnemy( entity );
	}

	void Game::killBullet( Entity entity )
	{
		m_bulletSpawner.killBullet( entity );
	}

	void Game::hit( Entity source
		, Entity target
		, uint32_t damage )
	{
		m_bulletSpawner.killBullet( source );

		auto & life = m_ecs.getComponentData< uint32_t >( target
			, m_ecs.getComponent( Ecs::LifeComponent ) );
		life.setValue( life.getValue() - std::min( life.getValue(), damage ) );

		if ( !life.getValue() )
		{
			m_enemySpawner.killEnemy( target );
		}
	}

	castor3d::GeometrySPtr Game::createEnemy( castor::String const & name
		, castor3d::MeshResPtr mesh )
	{
		auto baseNode = m_scene.getSceneNodeCache().add( name + cuT( "_Base" ) ).lock();
		baseNode->attachTo( *m_mapNode );
		auto node = m_scene.getSceneNodeCache().add( name ).lock();
		node->setOrientation( Quaternion::fromAxisAngle( Point3f{ 1, 0, 1 }, 10.0_degrees ) );
		node->attachTo( *baseNode );
		auto result = m_scene.getGeometryCache().create( name, m_scene, *node, mesh );
		m_scene.getGeometryCache().add( result );
		auto light = m_scene.getLightCache().create( name
			, m_scene
			, *node
			, m_scene.getLightsFactory()
			, LightType::ePoint );
		auto meshName = mesh.lock()->getName();

		if ( meshName.find( "Red" ) != castor::String::npos )
		{
			light->setColour( RgbColour::fromPredefined( PredefinedRgbColour::eRed ) );
		}
		else if ( meshName.find( "Green" ) != castor::String::npos )
		{
			light->setColour( RgbColour::fromPredefined( PredefinedRgbColour::eGreen ) );
		}
		else if ( meshName.find( "Purple" ) != castor::String::npos )
		{
			light->setColour( RgbColour::fromComponents( 0.569f, 0.514f, 0.875f ) );
		}
		else
		{
			light->setColour( RgbColour::fromComponents( 0.733, 0.537, 0.337 ) );
		}

		light->setIntensity( 0.8f, 1.0f );
		light->getPointLight()->setAttenuation( Point3f{ 1.0f, 0.1f, 0.0f } );
		m_scene.getLightCache().add( name, light );
		return result;
	}

	void Game::createShortRangeTower()
	{
		CU_Require( m_selectedCell && *m_selectedCell );
		if ( m_selectedCell->state == GridCell::State::eEmpty )
		{
			String name = cuT( "ShortRangeTower_" ) + std::to_string( m_selectedCell->x ) + cuT( "x" ) + std::to_string( m_selectedCell->y );
			auto tower = doCreateTower( name
				, *m_selectedCell
				, m_shortRangeTowerMesh
				, { cuT( "short_range_body" ), cuT( "short_range_accessories" ) } );
			String animName = cuT( "armature_short_range.1|attack" );
			auto anim = doCreateAnimation( tower
				, animName );

			if ( tower )
			{
				auto data = std::make_shared< AnimationData >( anim, animName );
				auto shoot = std::make_shared< AttackData >( 500_ms );
				m_selectedCell->entity = m_ecs.createTower( 1000_ms
					, 3u
					, 40.0f
					, 120.0f
					, 1u
					, tower
					, data
					, shoot );
				auto stateMachine = m_ecs.getComponentData< StateMachinePtr >( m_selectedCell->entity
					, m_ecs.getComponent( Ecs::StateComponent ) ).getValue();
				stateMachine->addState( tower::createShootingState( m_ecs, m_selectedCell->entity ) );
				doSelectEntity( m_selectedCell->entity );
			}
		}
	}

	void Game::createLongRangeTower()
	{
		CU_Require( m_selectedCell && *m_selectedCell );
		if ( m_selectedCell->state == GridCell::State::eEmpty )
		{
			String name = cuT( "LongRangeTower_" ) + std::to_string( m_selectedCell->x ) + cuT( "x" ) + std::to_string( m_selectedCell->y );
			auto tower = doCreateTower( name
				, *m_selectedCell
				, m_longRangeTowerMesh
				, { cuT( "splash_accessories" ), cuT( "splash_accessories" ), cuT( "splash_body" ) } );
			String animName = cuT( "armature_short_range.1|attack" );
			auto anim = doCreateAnimation( tower
				, animName );

			if ( tower )
			{
				auto data = std::make_shared< AnimationData >( anim, animName );
				auto shoot = std::make_shared< AttackData >( 800_ms );
				m_selectedCell->entity = m_ecs.createTower( 6000_ms
					, 5u
					, 100.0f
					, 200.0f
					, 1u
					, tower
					, data
					, shoot );
				auto stateMachine = m_ecs.getComponentData< StateMachinePtr >( m_selectedCell->entity
					, m_ecs.getComponent( Ecs::StateComponent ) ).getValue();
				stateMachine->addState( tower::createShootingState( m_ecs, m_selectedCell->entity ) );
				doSelectEntity( m_selectedCell->entity );
			}
		}
	}

	void Game::createBullet( Entity source
		, Entity target )
	{
		if ( m_bulletSpawner.hasFreeBullet() )
		{
			m_bulletSpawner.fireBullet( source
				, target );
		}
		else
		{
			m_bulletSpawner.fireBullet( source
				, target
				, doCreateBullet( source ) );
		}
	}

	Point3f Game::convert( castor::Point2i const & position )const
	{
		return Point3f( float( position[0] - int( m_grid.getWidth() ) / 2 ) * m_cellDimensions[0]
			, 0
			, float( position[1] - int( m_grid.getHeight() ) / 2 ) * m_cellDimensions[2] );
	}

	castor3d::SceneNodeRPtr Game::getEnemyNode( castor3d::GeometrySPtr geometry )
	{
		return geometry->getParent()->getParent();
	}

	castor3d::SceneNodeRPtr Game::getTowerNode( castor3d::GeometrySPtr geometry )
	{
		return geometry->getParent();
	}

	castor3d::SceneNodeRPtr Game::getBulletNode( castor3d::GeometrySPtr geometry )
	{
		return geometry->getParent();
	}

	void Game::doPrepareGrid()
	{
		auto prv = m_path.begin();

		if ( prv != m_path.end() )
		{
			auto cur = prv + 1;

			while ( cur != m_path.end() )
			{
				doPrepareGridLine( *prv, *cur, m_grid );

				if ( prv == m_path.begin() )
				{
					m_grid( prv->y, prv->x ).state = GridCell::State::eStart;
				}

				++prv;
				++cur;
			}

			doPrepareTarget( *prv, m_scene, m_grid );
		}
	}

	void Game::doAddTile( GridCell & cell
		, castor3d::MeshResPtr mesh
		, bool pickable )
	{
		String name = cuT( "Tile_" ) + std::to_string( cell.x ) + cuT( "x" ) + std::to_string( cell.y );
		auto node = m_scene.getSceneNodeCache().add( name ).lock();
		auto geometry = m_scene.getGeometryCache().create( name, m_scene, *node, mesh );
		node->setPosition( doConvert( Point2i{ cell.x, cell.y } ) + Point3f{ 0, m_cellDimensions[1] / 2, 0 } );
		node->attachTo( *m_mapNode );
		m_scene.getGeometryCache().add( geometry );
		cell.entity = m_ecs.createMapBlock( geometry, pickable );
	}

	void Game::doAddEmptyTile( GridCell & cell )
	{
		doAddTile( cell, m_emptyTileMesh, true );
		cell.state = GridCell::State::eEmpty;
	}

	void Game::doAddPathTile( GridCell & cell )
	{
		doAddTile( cell, m_pathTileMesh, false );
		cell.state = GridCell::State::ePath;
	}

	void Game::doAddTarget( GridCell & cell )
	{
		m_targetNode->setPosition( doConvert( Point2i{ cell.x, cell.y + 1 } ) );
		cell.state = GridCell::State::eTarget;
	}

	castor3d::GeometrySPtr Game::doCreateBullet( Entity source )
	{
		auto origin = Game::getTowerNode( m_ecs.getComponentData< castor3d::GeometrySPtr >( source
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue() );
		String name = cuT( "Bullet_" ) + toString( m_bulletSpawner.getBulletsCount() );
		auto node = m_scene.getSceneNodeCache().add( name ).lock();
		auto geometry = m_scene.getGeometryCache().create( name, m_scene, *node, m_bulletMesh );
		node->setPosition( origin->getDerivedPosition() );
		node->attachTo( *m_mapNode );

		for ( auto submesh : *geometry->getMesh().lock() )
		{
			geometry->setMaterial( *submesh, m_bulletMaterial.get() );
		}

		m_scene.getGeometryCache().add( geometry );
		return geometry;
	}

	GeometrySPtr Game::doCreateTower( String const & name
		, GridCell & cell
		, castor3d::MeshResPtr mesh
		, StringArray matNames )
	{
		auto node = m_scene.getSceneNodeCache().add( name ).lock();
		node->setPosition( doConvert( Point2i{ cell.x, cell.y } ) + Point3f{ 0, m_cellDimensions[1], 0 } );
		node->attachTo( *m_mapNode );
		auto tower = m_scene.getGeometryCache().create( name, m_scene, *node, mesh );
		node->setScale( Point3f{ 0.15, 0.15, 0.15 } );
		uint32_t index = 0u;

		for ( auto matName : matNames )
		{
			tower->setMaterial( *mesh.lock()->getSubmesh( index ), m_scene.getMaterialView().find( matName ).lock().get() );
			++index;
		}

		m_scene.getGeometryCache().add( tower );
		cell.state = GridCell::State::eTower;
		return tower;
	}

	 AnimatedObjectGroupSPtr Game::doCreateAnimation( GeometrySPtr geometry
		, String const & animName )
	{
		 CU_Require( geometry );
		 auto animGroup = m_scene.getAnimatedObjectGroupCache().add( geometry->getName(), m_scene ).lock();
		 std::chrono::milliseconds time{ 0 };
		 auto mesh = geometry->getMesh().lock();
		 CU_Require( mesh );

		 if ( !mesh->getAnimations().empty() )
		 {
			 auto object = animGroup->addObject( *mesh, *geometry, geometry->getName() + cuT( "_Mesh" ) );
			 time = std::max( time
				 , mesh->getAnimation( animName ).getLength() );
		 }

		 auto skeleton = mesh->getSkeleton();

		 if ( skeleton )
		 {
			 if ( !skeleton->getAnimations().empty() )
			 {
				 auto object = animGroup->addObject( *skeleton, *mesh, *geometry, geometry->getName() + cuT( "_Skeleton" ) );
				 time = std::max( time
					 , skeleton->getAnimation( animName ).getLength() );
			 }
		 }

		 animGroup->addAnimation( animName );
		 animGroup->setAnimationLooped( animName, false );
		 animGroup->startAnimation( animName );
		 animGroup->pauseAnimation( animName );
		 return animGroup;
	 }

	 void Game::doSelectMapBlock( Entity const & entity )
	 {
		GeometrySPtr geometry = doGetGeometry( entity );

		if ( geometry )
		{
			m_hud.updateTowerInfo( m_ecs, Entity{} );
			m_hud.showBuild();
			m_scene.getEngine()->postEvent( makeCpuFunctorEvent( EventType::ePostRender
				, [this, geometry, entity]()
				{
					m_selectedBlock.select( entity, geometry );
					m_selectedEntity.unselect();
				} ) );
		}
	}

	 void Game::doSelectTower( Entity const & entity )
	{
		GeometrySPtr geometry = doGetGeometry( entity );

		if ( geometry )
		{
			m_hud.updateTowerInfo( m_ecs, entity );
			m_hud.hideBuild();
			m_scene.getEngine()->postEvent( makeCpuFunctorEvent( EventType::ePostRender
				, [this, geometry, entity]()
				{
					m_selectedBlock.unselect();
					m_selectedEntity.select( entity, geometry );
				} ) );
		}
	}

	GeometrySPtr Game::doGetGeometry( Entity const & entity )
	{
		GeometrySPtr result;

		try
		{
			result = m_ecs.getComponentData< GeometrySPtr >( entity
				, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
		}
		catch ( Ecs::ComponentDataMatchException & exc )
		{
			castor::Logger::logWarning( exc.what() );
		}

		return result;
	}

	GridCell & Game::doFindCell( Entity const & entity )
	{
		static GridCell invalidCell{ 0u, 0u, GridCell::State::eInvalid };
		auto it = std::find_if( std::begin( m_grid )
			, std::end( m_grid )
			, [&entity]( GridCell const & lookup )
			{
				return lookup.entity == entity;
			} );

		if ( it != std::end( m_grid ) )
		{
			return *it;
		}

		return invalidCell;
	}

	bool Game::doSelectEntity( Entity const & entity )
	{
		bool result = false;

		if ( m_ecs.hasComponent( entity, m_ecs.getComponent( Ecs::DamageComponent ) ) )
		{
			result = true;
			doSelectTower( entity );
		}
		else
		{
			doSelectMapBlock( entity );
		}

		return result;
	}

	castor::Point3f Game::doConvert( castor::Point2i const & position )const
	{
		return castor::Point3f( float( position[0] - int( m_grid.getWidth() ) / 2 ) * m_cellDimensions[0]
			, 0
			, float( position[1] - int( m_grid.getHeight() ) / 2 ) * m_cellDimensions[2] );
	}
}
