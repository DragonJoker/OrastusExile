#include "Game.hpp"

#include "Ecs/AnimationData.hpp"
#include "Ecs/AttackData.hpp"
#include "ECS/WalkData.hpp"
#include "State/EnemyState.hpp"
#include "State/StateMachine.hpp"
#include "State/TowerState.hpp"

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
		void doPrepareGridLine( GridPathNode const & p_prv
			, GridPathNode const & p_cur
			, Grid & p_grid )
		{
			if ( p_prv.m_x != p_cur.m_x )
			{
				for ( auto x = std::min( p_prv.m_x, p_cur.m_x ); x < std::max( p_prv.m_x, p_cur.m_x ); ++x )
				{
					p_grid( p_prv.m_y - 1, x - 1 ).m_state = GridCell::State::ePath;
					p_grid( p_prv.m_y - 1, x - 0 ).m_state = GridCell::State::ePath;
					p_grid( p_prv.m_y - 0, x - 1 ).m_state = GridCell::State::ePath;
					p_grid( p_prv.m_y - 0, x - 0 ).m_state = GridCell::State::ePath;
				}
			}
			else
			{
				for ( auto y = std::min( p_prv.m_y, p_cur.m_y ); y <= std::max( p_prv.m_y, p_cur.m_y ); ++y )
				{
					p_grid( y - 1, p_prv.m_x - 1 ).m_state = GridCell::State::ePath;
					p_grid( y - 1, p_prv.m_x - 0 ).m_state = GridCell::State::ePath;
					p_grid( y - 0, p_prv.m_x - 1 ).m_state = GridCell::State::ePath;
					p_grid( y - 0, p_prv.m_x - 0 ).m_state = GridCell::State::ePath;
				}
			}
		}

		void doPrepareTarget( GridPathNode const & p_cur
			, Scene & p_scene
			, Grid & p_grid )
		{
			for ( uint32_t x = p_cur.m_x - 2; x <= p_cur.m_x + 2; ++x )
			{
				for ( uint32_t y = p_cur.m_y - 1; y <= p_cur.m_y + 3; ++y )
				{
					p_grid( y, x ).m_state = GridCell::State::ePath;
				}
			}

			p_grid( p_cur.m_y, p_cur.m_x ).m_state = GridCell::State::eTarget;
		}
	}

	//*********************************************************************************************

	void Game::SelectedEntity::select( Entity const & p_entity
		, castor3d::GeometrySPtr p_geometry )
	{
		unselect();

		if ( p_entity && p_geometry )
		{
			m_entity = p_entity;
			m_geometry = p_geometry;
			m_materials.clear();
			auto & l_materials = m_geometry->getScene()->getEngine()->getMaterialCache();

			for ( auto l_submesh : *m_geometry->getMesh().lock() )
			{
				auto l_old = m_geometry->getMaterial( *l_submesh );
				auto l_new = l_materials.find( l_old->getName() + cuT( "_sel" ) ).lock();
				CU_Require( l_new );
				m_geometry->setMaterial( *l_submesh, l_new.get() );
				m_materials.push_back( l_old );
			}
		}
	}

	void Game::SelectedEntity::unselect()
	{
		if ( m_entity && m_geometry )
		{
			uint32_t l_index{ 0u };
			auto l_mesh = m_geometry->getMesh().lock();

			for ( auto l_material : m_materials )
			{
				m_geometry->setMaterial( *l_mesh->getSubmesh( l_index++ ), l_material );
			}

			m_materials.clear();
			m_geometry.reset();
			m_entity = Entity{};
		}
	}

	//*********************************************************************************************

	auto constexpr s_faceIndex = 5u;

	void Game::SelectedBlock::select( Entity const & p_entity
		, castor3d::GeometrySPtr p_geometry )
	{
		unselect();

		if ( p_entity && p_geometry )
		{
			m_entity = p_entity;
			m_geometry = p_geometry;
			auto & l_submesh = *m_geometry->getMesh().lock()->getSubmesh( s_faceIndex );
			m_material = p_geometry->getMaterial( l_submesh );
			auto & l_materials = m_geometry->getScene()->getEngine()->getMaterialCache();
			auto l_new = l_materials.find( m_material->getName() + cuT( "_sel" ) ).lock();
			m_geometry->setMaterial( l_submesh, l_new.get() );
		}
	}

	void Game::SelectedBlock::unselect()
	{
		if ( m_entity && m_geometry )
		{
			auto & l_submesh = *m_geometry->getMesh().lock()->getSubmesh( s_faceIndex );
			m_geometry->setMaterial( l_submesh, m_material );
			m_material = nullptr;
			m_geometry.reset();
			m_entity = Entity{};
		}
	}

	//*********************************************************************************************

	Game::Game( castor3d::Scene & p_scene )
		: m_scene{ p_scene }
		, m_hud{ *this, p_scene }
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
		m_mapCubeMesh = m_scene.getMeshCache().find( cuT( "MapCube" ) );
		m_mapCubeMaterial = m_scene.getMaterialView().find( cuT( "MapCube" ) ).lock();
		m_shortRangeTowerMesh = m_scene.getMeshCache().find( cuT( "ShortRange" ) ).lock();
		m_longRangeTowerMesh = m_scene.getMeshCache().find( cuT( "HeavySplash" ) ).lock();
		m_enemyCubeMesh = m_scene.getMeshCache().find( cuT( "EnemyCube" ) ).lock();
		m_enemyCubeMaterial = m_scene.getMaterialView().find( cuT( "EnemyCube" ) ).lock();
		m_bulletMesh = m_scene.getMeshCache().find( cuT( "Bullet" ) ).lock();
		m_bulletMaterial = m_scene.getMaterialView().find( cuT( "Bullet" ) ).lock();
		m_targetNode = m_scene.getSceneNodeCache().find( cuT( "Target" ) ).lock();
		auto mesh = m_mapCubeMesh.lock();
		m_cellDimensions[0] = mesh->getBoundingBox().getMax()[0] - mesh->getBoundingBox().getMin()[0];
		m_cellDimensions[1] = mesh->getBoundingBox().getMax()[1] - mesh->getBoundingBox().getMin()[1];
		m_cellDimensions[2] = mesh->getBoundingBox().getMax()[2] - mesh->getBoundingBox().getMin()[2];
		m_hud.initialise();
		m_state = State::eInitial;
	}

	void Game::reset()
	{
		Grid l_grid;
		std::swap( m_grid, l_grid );
		m_state = State::eStarted;
		m_bulletSpawner.reset();
		m_enemySpawner.reset();
	}

	void Game::start()
	{
		doPrepareGrid();

		for ( auto & l_cell : m_grid )
		{
			if ( l_cell.m_state == GridCell::State::eEmpty )
			{
				doAddMapCube( l_cell );
			}
		}

		auto & l_node = *m_path.rbegin();
		doAddTarget( m_grid( l_node.m_y, l_node.m_x ) );

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

	Entity Game::select( castor3d::GeometrySPtr p_geometry )
	{
		Entity l_result;
		auto l_entity = m_ecs.findEntity( m_ecs.getComponent( Ecs::GeometryComponent ), p_geometry );
		m_selectedCell = &doFindCell( l_entity );
		l_entity = m_selectedCell->m_entity;

		if ( m_ecs.hasComponent( l_entity, m_ecs.getComponent( Ecs::PickableComponent ) ) )
		{
			if ( doSelectEntity( l_entity ) )
			{
				l_result = l_entity;
			}
		}
		else
		{
			unselect();
		}

		return l_result;
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

	void Game::killEnemy( Entity p_entity )
	{
		m_enemySpawner.killEnemy( p_entity );
	}

	void Game::killBullet( Entity p_entity )
	{
		m_bulletSpawner.killBullet( p_entity );
	}

	void Game::hit( Entity p_source
		, Entity p_target
		, uint32_t p_damage )
	{
		m_bulletSpawner.killBullet( p_source );

		auto & l_life = m_ecs.getComponentData< uint32_t >( p_target
			, m_ecs.getComponent( Ecs::LifeComponent ) );
		l_life.setValue( l_life.getValue() - std::min( l_life.getValue(), p_damage ) );

		if ( !l_life.getValue() )
		{
			m_enemySpawner.killEnemy( p_target );
		}
	}

	castor3d::GeometrySPtr Game::createEnemy( castor::String const & p_name )
	{
		auto l_baseNode = m_scene.getSceneNodeCache().add( p_name + cuT( "_Base" ) ).lock();
		l_baseNode->attachTo( *m_mapNode );
		auto l_node = m_scene.getSceneNodeCache().add( p_name ).lock();
		l_node->setOrientation( Quaternion::fromAxisAngle( Point3f{ 1, 0, 1 }, 45.0_degrees ) );
		l_node->attachTo( *l_baseNode );
		auto l_geometry = m_scene.getGeometryCache().create( p_name, m_scene, *l_node, m_enemyCubeMesh );

		for ( auto l_submesh : *l_geometry->getMesh().lock() )
		{
			l_geometry->setMaterial( *l_submesh, m_enemyCubeMaterial.get() );
		}

		m_scene.getGeometryCache().add( l_geometry );
		auto l_light = m_scene.getLightCache().create( p_name
			, m_scene
			, *l_node
			, m_scene.getLightsFactory()
			, LightType::ePoint );
		l_light->setColour( RgbColour::fromPredefined( PredefinedRgbColour::eRed ) );
		l_light->setIntensity( 0.8f, 1.0f );
		l_light->getPointLight()->setAttenuation( Point3f{ 1.0f, 0.1f, 0.0f } );
		m_scene.getLightCache().add( p_name, l_light );
		return l_geometry;
	}

	void Game::createShortRangeTower()
	{
		CU_Require( m_selectedCell && *m_selectedCell );
		if ( m_selectedCell->m_state == GridCell::State::eEmpty )
		{
			String l_name = cuT( "ShortRangeTower_" ) + std::to_string( m_selectedCell->m_x ) + cuT( "x" ) + std::to_string( m_selectedCell->m_y );
			auto l_tower = doCreateTower( l_name
				, *m_selectedCell
				, m_shortRangeTowerMesh
				, { cuT( "short_range_body" ), cuT( "short_range_accessories" ) } );
			String l_animName = cuT( "armature_short_range.1|attack" );
			auto l_anim = doCreateAnimation( l_tower
				, l_animName );

			if ( l_tower )
			{
				auto l_data = std::make_shared< AnimationData >( l_anim, l_animName );
				auto l_shoot = std::make_shared< AttackData >( 500_ms );
				m_selectedCell->m_entity = m_ecs.createTower( 1000_ms
					, 3u
					, 40.0f
					, 120.0f
					, 1u
					, l_tower
					, l_data
					, l_shoot );
				auto l_stateMachine = m_ecs.getComponentData< StateMachinePtr >( m_selectedCell->m_entity
					, m_ecs.getComponent( Ecs::StateComponent ) ).getValue();
				l_stateMachine->addState( tower::createShootingState( m_ecs, m_selectedCell->m_entity ) );
				doSelectEntity( m_selectedCell->m_entity );
			}
		}
	}

	void Game::createLongRangeTower()
	{
		CU_Require( m_selectedCell && *m_selectedCell );
		if ( m_selectedCell->m_state == GridCell::State::eEmpty )
		{
			String l_name = cuT( "LongRangeTower_" ) + std::to_string( m_selectedCell->m_x ) + cuT( "x" ) + std::to_string( m_selectedCell->m_y );
			auto l_tower = doCreateTower( l_name
				, *m_selectedCell
				, m_longRangeTowerMesh
				, { cuT( "splash_accessories" ), cuT( "splash_accessories" ), cuT( "splash_body" ) } );
			String l_animName = cuT( "armature_short_range.1|attack" );
			auto l_anim = doCreateAnimation( l_tower
				, l_animName );

			if ( l_tower )
			{
				auto l_data = std::make_shared< AnimationData >( l_anim, l_animName );
				auto l_shoot = std::make_shared< AttackData >( 800_ms );
				m_selectedCell->m_entity = m_ecs.createTower( 6000_ms
					, 5u
					, 100.0f
					, 200.0f
					, 1u
					, l_tower
					, l_data
					, l_shoot );
				auto l_stateMachine = m_ecs.getComponentData< StateMachinePtr >( m_selectedCell->m_entity
					, m_ecs.getComponent( Ecs::StateComponent ) ).getValue();
				l_stateMachine->addState( tower::createShootingState( m_ecs, m_selectedCell->m_entity ) );
				doSelectEntity( m_selectedCell->m_entity );
			}
		}
	}

	void Game::createBullet( Entity p_source
		, Entity p_target )
	{
		if ( m_bulletSpawner.hasFreeBullet() )
		{
			m_bulletSpawner.fireBullet( p_source
				, p_target );
		}
		else
		{
			m_bulletSpawner.fireBullet( p_source
				, p_target
				, doCreateBullet( p_source ) );
		}
	}

	Point3f Game::convert( castor::Point2i const & p_position )const
	{
		return Point3f( float( p_position[0] - int( m_grid.getWidth() ) / 2 ) * m_cellDimensions[0]
			, 0
			, float( p_position[1] - int( m_grid.getHeight() ) / 2 ) * m_cellDimensions[2] );
	}

	castor3d::SceneNodeRPtr Game::getEnemyNode( castor3d::GeometrySPtr p_geometry )
	{
		return p_geometry->getParent()->getParent();
	}

	castor3d::SceneNodeRPtr Game::getTowerNode( castor3d::GeometrySPtr p_geometry )
	{
		return p_geometry->getParent();
	}

	castor3d::SceneNodeRPtr Game::getBulletNode( castor3d::GeometrySPtr p_geometry )
	{
		return p_geometry->getParent();
	}

	void Game::doPrepareGrid()
	{
		auto l_prv = m_path.begin();

		if ( l_prv != m_path.end() )
		{
			auto l_cur = l_prv + 1;

			while ( l_cur != m_path.end() )
			{
				doPrepareGridLine( *l_prv, *l_cur, m_grid );

				if ( l_prv == m_path.begin() )
				{
					m_grid( l_prv->m_y, l_prv->m_x ).m_state = GridCell::State::eStart;
				}

				++l_prv;
				++l_cur;
			}

			doPrepareTarget( *l_prv, m_scene, m_grid );
		}
	}

	void Game::doAddMapCube( GridCell & p_cell )
	{
		String l_name = cuT( "MapCube_" ) + std::to_string( p_cell.m_x ) + cuT( "x" ) + std::to_string( p_cell.m_y );
		auto l_node = m_scene.getSceneNodeCache().add( l_name ).lock();
		auto l_geometry = m_scene.getGeometryCache().create( l_name, m_scene, *l_node, m_mapCubeMesh );
		l_node->setPosition( doConvert( Point2i{ p_cell.m_x, p_cell.m_y } ) + Point3f{ 0, m_cellDimensions[1] / 2, 0 } );
		l_node->attachTo( *m_mapNode );

		for ( auto l_submesh : *l_geometry->getMesh().lock() )
		{
			l_geometry->setMaterial( *l_submesh, m_mapCubeMaterial.get() );
		}

		m_scene.getGeometryCache().add( l_geometry );
		p_cell.m_entity = m_ecs.createMapBlock( l_geometry );
		m_lastMapCube = l_geometry;
		p_cell.m_state = GridCell::State::eEmpty;
	}

	void Game::doAddTarget( GridCell & p_cell )
	{
		m_targetNode->setPosition( doConvert( Point2i{ p_cell.m_x, p_cell.m_y + 1 } ) );
		p_cell.m_state = GridCell::State::eTarget;
	}

	castor3d::GeometrySPtr Game::doCreateBullet( Entity p_source )
	{
		auto l_origin = Game::getTowerNode( m_ecs.getComponentData< castor3d::GeometrySPtr >( p_source
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue() );
		String l_name = cuT( "Bullet_" ) + toString( m_bulletSpawner.getBulletsCount() );
		auto l_node = m_scene.getSceneNodeCache().add( l_name ).lock();
		auto l_geometry = m_scene.getGeometryCache().create( l_name, m_scene, *l_node, m_bulletMesh );
		l_node->setPosition( l_origin->getDerivedPosition() );
		l_node->attachTo( *m_mapNode );

		for ( auto l_submesh : *l_geometry->getMesh().lock() )
		{
			l_geometry->setMaterial( *l_submesh, m_bulletMaterial.get() );
		}

		m_scene.getGeometryCache().add( l_geometry );
		return l_geometry;
	}

	GeometrySPtr Game::doCreateTower( String const & p_name
		, GridCell & p_cell
		, castor3d::MeshResPtr p_mesh
		, StringArray p_matNames )
	{
		auto l_node = m_scene.getSceneNodeCache().add( p_name ).lock();
		l_node->setPosition( doConvert( Point2i{ p_cell.m_x, p_cell.m_y } ) + Point3f{ 0, m_cellDimensions[1], 0 } );
		l_node->attachTo( *m_mapNode );
		auto l_tower = m_scene.getGeometryCache().create( p_name, m_scene, *l_node, p_mesh );
		l_node->setScale( Point3f{ 0.15, 0.15, 0.15 } );
		uint32_t l_index = 0u;

		for ( auto l_name : p_matNames )
		{
			l_tower->setMaterial( *p_mesh.lock()->getSubmesh( l_index ), m_scene.getMaterialView().find( l_name ).lock().get() );
			++l_index;
		}

		m_scene.getGeometryCache().add( l_tower );
		p_cell.m_state = GridCell::State::eTower;
		return l_tower;
	}

	 AnimatedObjectGroupSPtr Game::doCreateAnimation( GeometrySPtr p_geometry
		, String const & p_animName )
	{
		 CU_Require( p_geometry );
		 auto l_animGroup = m_scene.getAnimatedObjectGroupCache().add( p_geometry->getName(), m_scene ).lock();
		 std::chrono::milliseconds l_time{ 0 };
		 auto l_mesh = p_geometry->getMesh().lock();
		 CU_Require( l_mesh );

		 if ( !l_mesh->getAnimations().empty() )
		 {
			 auto l_object = l_animGroup->addObject( *l_mesh, *p_geometry, p_geometry->getName() + cuT( "_Mesh" ) );
			 l_time = std::max( l_time
				 , l_mesh->getAnimation( p_animName ).getLength() );
		 }

		 auto l_skeleton = l_mesh->getSkeleton();

		 if ( l_skeleton )
		 {
			 if ( !l_skeleton->getAnimations().empty() )
			 {
				 auto l_object = l_animGroup->addObject( *l_skeleton, *l_mesh, *p_geometry, p_geometry->getName() + cuT( "_Skeleton" ) );
				 l_time = std::max( l_time
					 , l_skeleton->getAnimation( p_animName ).getLength() );
			 }
		 }

		 l_animGroup->addAnimation( p_animName );
		 l_animGroup->setAnimationLooped( p_animName, false );
		 l_animGroup->startAnimation( p_animName );
		 l_animGroup->pauseAnimation( p_animName );
		 return l_animGroup;
	 }

	 void Game::doSelectMapBlock( Entity const & p_entity )
	 {
		GeometrySPtr l_geometry = doGetGeometry( p_entity );

		if ( l_geometry )
		{
			m_hud.updateTowerInfo( m_ecs, Entity{} );
			m_hud.showBuild();
			m_scene.getEngine()->postEvent( makeCpuFunctorEvent( EventType::ePostRender
				, [this, l_geometry, p_entity]()
				{
					m_selectedBlock.select( p_entity, l_geometry );
					m_selectedEntity.unselect();
				} ) );
		}
	}

	 void Game::doSelectTower( Entity const & p_entity )
	{
		GeometrySPtr l_geometry = doGetGeometry( p_entity );

		if ( l_geometry )
		{
			m_hud.updateTowerInfo( m_ecs, p_entity );
			m_hud.hideBuild();
			m_scene.getEngine()->postEvent( makeCpuFunctorEvent( EventType::ePostRender
				, [this, l_geometry, p_entity]()
				{
					m_selectedBlock.unselect();
					m_selectedEntity.select( p_entity, l_geometry );
				} ) );
		}
	}

	GeometrySPtr Game::doGetGeometry( Entity const & p_entity )
	{
		GeometrySPtr l_result;

		try
		{
			l_result = m_ecs.getComponentData< GeometrySPtr >( p_entity
				, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
		}
		catch ( Ecs::ComponentDataMatchException & p_exc )
		{
			castor::Logger::logWarning( p_exc.what() );
		}

		return l_result;
	}

	GridCell & Game::doFindCell( Entity const & p_entity )
	{
		static GridCell l_invalidCell{ 0u, 0u, GridCell::State::eInvalid };
		auto l_it = std::find_if( std::begin( m_grid )
			, std::end( m_grid )
			, [&p_entity]( GridCell const & p_cell )
			{
				return p_cell.m_entity == p_entity;
			} );

		if ( l_it != std::end( m_grid ) )
		{
			return *l_it;
		}

		return l_invalidCell;
	}

	bool Game::doSelectEntity( Entity const & p_entity )
	{
		bool l_result = false;

		if ( m_ecs.hasComponent( p_entity, m_ecs.getComponent( Ecs::DamageComponent ) ) )
		{
			l_result = true;
			doSelectTower( p_entity );
		}
		else
		{
			doSelectMapBlock( p_entity );
		}

		return l_result;
	}

	castor::Point3f Game::doConvert( castor::Point2i const & p_position )const
	{
		return castor::Point3f( float( p_position[0] - int( m_grid.getWidth() ) / 2 ) * m_cellDimensions[0]
			, 0
			, float( p_position[1] - int( m_grid.getHeight() ) / 2 ) * m_cellDimensions[2] );
	}
}
