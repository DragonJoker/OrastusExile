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

#include <random>

namespace orastus
{
	//*********************************************************************************************

	namespace
	{
		enum class Direction
		{
			North,
			South,
			East,
			West,
		};

		Direction doGetDirection( GridPathNode const & from
			, GridPathNode const & to )
		{
			if ( from.y < to.y )
			{
				return Direction::South;
			}

			if ( from.y > to.y )
			{
				return Direction::North;
			}

			if ( from.x < to.x )
			{
				return Direction::West;
			}

			return Direction::East;
		}

		GridCell::State doGetPathType( GridPathNode const & prv
			, GridPathNode const & cur
			, GridPathNode const * from )
		{
			if ( from )
			{
				auto fromDir = doGetDirection( *from, prv );
				auto toDir = doGetDirection( cur, prv );

				switch ( fromDir )
				{
				case Direction::North:
					switch ( toDir )
					{
					case Direction::East:
						return GridCell::State::ePathTurnNE;
					case Direction::West:
						return GridCell::State::ePathTurnWN;
					default:
						return GridCell::State::ePathStraightSN;
					}
				case Direction::South:
					switch ( toDir )
					{
					case Direction::East:
						return GridCell::State::ePathTurnES;
					case Direction::West:
						return GridCell::State::ePathTurnSW;
					default:
						return GridCell::State::ePathStraightSN;
					}
				case Direction::East:
					switch ( toDir )
					{
					case Direction::North:
						return GridCell::State::ePathTurnNE;
					case Direction::South:
						return GridCell::State::ePathTurnES;
					default:
						return GridCell::State::ePathStraightEW;
					}
				case Direction::West:
					switch ( toDir )
					{
					case Direction::North:
						return GridCell::State::ePathTurnWN;
					case Direction::South:
						return GridCell::State::ePathTurnSW;
					default:
						return GridCell::State::ePathStraightEW;
					}
				}
			}

			return GridCell::State::eStart;
		}

		void doPrepareGridLine( GridPathNode const & prv
			, GridPathNode const & cur
			, GridPathNode const *& from
			, Grid & grid )
		{
			grid( prv.y, prv.x ).state = doGetPathType( prv, cur, from );

			if ( prv.x != cur.x )
			{
				auto begin = std::min( prv.x, cur.x );
				auto end = std::max( prv.x, cur.x );

				for ( auto x = begin + 1u; x < end; ++x )
				{
					grid( cur.y, x ).state = GridCell::State::ePathStraightEW;
				}
			}
			else
			{
				auto begin = std::min( prv.y, cur.y );
				auto end = std::max( prv.y, cur.y );

				for ( auto y = begin + 1u; y < end; ++y )
				{
					grid( y, cur.x ).state = GridCell::State::ePathStraightSN;
				}
			}

			from = &prv;
		}

		int32_t constexpr targetXMin = -2;
		int32_t constexpr targetXMax = 2;
		int32_t constexpr targetYMin = -1;
		int32_t constexpr targetYMax = 3;

		void doPrepareTarget( GridPathNode const & cur
			, castor3d::Scene & scene
			, Grid & grid )
		{
			for ( uint32_t x = cur.x + targetXMin; x <= cur.x + targetXMax; ++x )
			{
				for ( uint32_t y = cur.y + targetYMin; y <= cur.y + targetYMax; ++y )
				{
					grid( y, x ).state = GridCell::State::ePathArea;
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
		, m_path{ { 19, 0 }
			, { 19, 3 }
			, { 11, 3 }
			, { 11, 7 }
			, { 23, 7 }
			, { 23, 11 }
			, { 13, 11 }
			, { 13, 15 }
			, { 25, 15 }
			, { 25, 19 }
			, { 15, 19 }
			, { 15, 23 }
			, { 17, 23 }
			, { 17, 27 } }
		, m_mapNode{ m_scene.getSceneNodeCache().find( cuT( "MapBase" ) ).lock() }
		, m_emptyTileMesh{ m_scene.getMeshCache().find( cuT( "EmptyTile" ) ) }
		, m_cellDimensions{ m_emptyTileMesh.lock()->getBoundingBox().getDimensions() }
		, m_pathStartTileMesh{ m_scene.getMeshCache().find( cuT( "PathStartTile" ) ) }
		, m_pathStraightTileMesh{ m_scene.getMeshCache().find( cuT( "PathStraightTile" ) ) }
		, m_pathTurnTileMesh{ m_scene.getMeshCache().find( cuT( "PathTurnTile" ) ) }
		, m_pathAreaTileMesh{ m_scene.getMeshCache().find( cuT( "PathAreaTile" ) ) }
		, m_towerBaseMesh{ m_scene.getMeshCache().find( cuT( "TowerBase" ) ).lock() }
		, m_shortRangeTowerMesh{ m_scene.getMeshCache().find( cuT( "Ballista" ) ).lock() }
		, m_longRangeTowerMesh{ m_scene.getMeshCache().find( cuT( "Cannon" ) ).lock() }
		, m_bulletMesh{ m_scene.getMeshCache().find( cuT( "Bullet" ) ).lock() }
		, m_bulletMaterial{ m_scene.getMaterialView().find( cuT( "Bullet" ) ).lock() }
		, m_cowMesh{ m_scene.getMeshCache().find( cuT( "Cow" ) ).lock() }
		, m_enemySpawner{ m_ecs, *this }
		, m_bulletSpawner{ m_ecs, *this }
	{
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
				doAddPathTile( cell
					, m_pathStartTileMesh
					, castor::Quaternion::fromAxisAngle( castor::Point3f{ 0.0, 1.0, 0.0 }, 180.0_degrees ) );
				break;
			case GridCell::State::ePathStraightSN:
				doAddPathTile( cell
					, m_pathStraightTileMesh
					, castor::Quaternion::identity() );
				break;
			case GridCell::State::ePathStraightEW:
				doAddPathTile( cell
					, m_pathStraightTileMesh
					, castor::Quaternion::fromAxisAngle( castor::Point3f{ 0.0, 1.0, 0.0 }, 90.0_degrees ) );
				break;
			case GridCell::State::ePathTurnNE:
				doAddPathTile( cell
					, m_pathTurnTileMesh
					, castor::Quaternion::fromAxisAngle( castor::Point3f{ 0.0, 1.0, 0.0 }, 180.0_degrees ) );
				break;
			case GridCell::State::ePathTurnES:
				doAddPathTile( cell
					, m_pathTurnTileMesh
					, castor::Quaternion::fromAxisAngle( castor::Point3f{ 0.0, 1.0, 0.0 }, 270.0_degrees ) );
				break;
			case GridCell::State::ePathTurnSW:
				doAddPathTile( cell
					, m_pathTurnTileMesh
					, castor::Quaternion::identity() );
				break;
			case GridCell::State::ePathTurnWN:
				doAddPathTile( cell
					, m_pathTurnTileMesh
					, castor::Quaternion::fromAxisAngle( castor::Point3f{ 0.0, 1.0, 0.0 }, 90.0_degrees ) );
				break;
			case GridCell::State::ePathArea:
				doAddPathTile( cell
					, m_pathAreaTileMesh
					, castor::Quaternion::identity() );
				break;
			case GridCell::State::eTarget:
				doAddPathTile( cell
					, m_pathAreaTileMesh
					, castor::Quaternion::identity() );
				break;
			default:
				break;
			}
		}

		auto & node = *m_path.rbegin();
		doAddTargets( m_grid( node.y, node.x ) );

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
			m_elapsed = std::min( 40_ms
				, std::chrono::duration_cast< castor::Milliseconds >( Clock::now() - m_saved ) );
			m_ecs.update( *this, m_elapsed );
			m_enemySpawner.update( m_elapsed );
			m_hud.update();
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

		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
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

	void Game::enemyArrived( Entity entity )
	{
		loseLife();
		m_enemySpawner.enemyArrived( entity );
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

	void Game::loseLife()
	{
	}

	castor3d::GeometrySPtr Game::createEnemy( castor::String const & name
		, castor3d::MeshResPtr mesh )
	{
		auto baseNode = m_scene.getSceneNodeCache().add( name + cuT( "_Base" ) ).lock();
		baseNode->attachTo( *m_mapNode );
		auto node = m_scene.getSceneNodeCache().add( name ).lock();
		node->setOrientation( castor::Quaternion::fromAxisAngle( castor::Point3f{ 1, 0, 1 }, 10.0_degrees ) );
		node->attachTo( *baseNode );
		auto result = m_scene.getGeometryCache().create( name, m_scene, *node, mesh );
		m_scene.getGeometryCache().add( result );
		auto light = m_scene.getLightCache().create( name
			, m_scene
			, *node
			, m_scene.getLightsFactory()
			, castor3d::LightType::ePoint );
		auto meshName = mesh.lock()->getName();

		if ( meshName.find( "Red" ) != castor::String::npos )
		{
			light->setColour( castor::RgbColour::fromPredefined( castor::PredefinedRgbColour::eRed ) );
		}
		else if ( meshName.find( "Green" ) != castor::String::npos )
		{
			light->setColour( castor::RgbColour::fromPredefined( castor::PredefinedRgbColour::eGreen ) );
		}
		else if ( meshName.find( "Purple" ) != castor::String::npos )
		{
			light->setColour( castor::RgbColour::fromComponents( 0.569f, 0.514f, 0.875f ) );
		}
		else
		{
			light->setColour( castor::RgbColour::fromComponents( 0.733, 0.537, 0.337 ) );
		}

		light->setIntensity( 4.0f, 5.0f );
		light->getPointLight()->setAttenuation( castor::Point3f{ 1.0f, 0.1f, 0.0f } );
		m_scene.getLightCache().add( name, light );
		return result;
	}

	void Game::createShortRangeTower()
	{
		CU_Require( m_selectedCell && *m_selectedCell );
		if ( m_selectedCell->state == GridCell::State::eEmpty )
		{
			String name = cuT( "ShortRangeTower_" ) + std::to_string( m_selectedCell->x ) + cuT( "x" ) + std::to_string( m_selectedCell->y );
			doCreateTowerBase( name + "Base"
				, *m_selectedCell
				, m_towerBaseMesh );
			auto tower = doCreateTower( name
				, *m_selectedCell
				, *m_towerBaseMesh.lock()
				, m_shortRangeTowerMesh );

			if ( tower )
			{
				m_selectedCell->entity = m_ecs.createTower( 700_ms
					, 3u
					, 40.0f
					, 240.0f
					, 1u
					, tower
					, nullptr
					, std::make_unique< AttackData >( 0_ms ) );
				auto & stateMachine = m_ecs.getComponentData< StateMachinePtr >( m_selectedCell->entity
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
			doCreateTowerBase( name + "Base"
				, *m_selectedCell
				, m_towerBaseMesh );
			auto tower = doCreateTower( name
				, *m_selectedCell
				, *m_towerBaseMesh.lock()
				, m_longRangeTowerMesh );

			if ( tower )
			{
				m_selectedCell->entity = m_ecs.createTower( 2000_ms
					, 5u
					, 100.0f
					, 200.0f
					, 1u
					, tower
					, nullptr
					, std::make_unique< AttackData >( 0_ms ) );
				auto & stateMachine = m_ecs.getComponentData< StateMachinePtr >( m_selectedCell->entity
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

	castor::Point3f Game::convert( castor::Point2i const & position )const
	{
		return castor::Point3f( float( position[0] - int( m_grid.getWidth() ) / 2 ) * m_cellDimensions[0]
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
			auto cur = std::next( prv );
			GridPathNode const * from{};

			while ( cur != m_path.end() )
			{
				doPrepareGridLine( *prv, *cur, from, m_grid );
				++prv;
				++cur;
			}

			doPrepareTarget( *prv, m_scene, m_grid );
		}
	}

	void Game::doAddTile( GridCell & cell
		, castor3d::MeshResPtr mesh
		, castor::Quaternion const & orientation
		, bool pickable )
	{
		String name = cuT( "Tile_" ) + std::to_string( cell.x ) + cuT( "x" ) + std::to_string( cell.y );
		auto node = m_scene.getSceneNodeCache().add( name ).lock();
		auto geometry = m_scene.getGeometryCache().create( name, m_scene, *node, mesh );
		node->setPosition( doConvert( castor::Point2i{ cell.x, cell.y } )
			+ castor::Point3f{ 0, m_cellDimensions[1] / 2, 0 } );
		node->setOrientation( orientation );
		node->attachTo( *m_mapNode );
		m_scene.getGeometryCache().add( geometry );
		cell.entity = m_ecs.createMapBlock( geometry, pickable );
	}

	void Game::doAddEmptyTile( GridCell & cell )
	{
		doAddTile( cell
			, m_emptyTileMesh
			, castor::Quaternion::identity()
			, true );
	}

	void Game::doAddPathTile( GridCell & cell
		, castor3d::MeshResPtr mesh
		, castor::Quaternion const & orientation )
	{
		doAddTile( cell
			, mesh
			, orientation
			, false );
	}

	void Game::doAddTargets( GridCell & cell )
	{
		uint32_t index = 0u;
		std::random_device r;
		std::default_random_engine e{ r() };
		std::uniform_real_distribution< float > distribution{ 0.0f, 360.0f };

		for ( uint32_t x = cell.x + targetXMin; x <= cell.x + targetXMax; ++x )
		{
			for ( uint32_t y = cell.y + targetYMin; y <= cell.y + targetYMax; ++y )
			{
				auto name = "Target" + castor::string::toString( index++ );
				auto node = m_scene.getSceneNodeCache().add( name ).lock();
				node->setPosition( doConvert( castor::Point2i{ x, y } )
					+ castor::Point3f{ 0, getCellHeight() / 5.0f, 0 } );
				node->yaw( castor::Angle::fromDegrees( distribution( e ) ) );
				node->setScale( { 0.05f, 0.05f, 0.05f } );
				node->attachTo( *m_mapNode );
				auto cow = m_scene.getGeometryCache().create( name, m_scene, *node, m_cowMesh );
				doCreateAnimation( cow, "C4D Animation Take", true, false );
				m_scene.getGeometryCache().add( cow );
			}
		}

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

	castor3d::GeometrySPtr Game::doCreateTowerBase( castor::String const & name
		, GridCell & cell
		, castor3d::MeshResPtr mesh )
	{
		auto node = m_scene.getSceneNodeCache().add( name ).lock();
		node->setPosition( doConvert( castor::Point2i{ cell.x, cell.y } )
			+ castor::Point3f{ 0, m_cellDimensions[1], 0 }
			+ castor::Point3f{ 0, mesh.lock().get()->getBoundingBox().getDimensions()[1] / 2.0f, 0 } );
		node->attachTo( *m_mapNode );
		auto base = m_scene.getGeometryCache().create( name, m_scene, *node, mesh );
		m_scene.getGeometryCache().add( base );
		return base;
	}

	castor3d::GeometrySPtr Game::doCreateTower( String const & name
		, GridCell & cell
		, castor3d::Mesh const & base
		, castor3d::MeshResPtr mesh )
	{
		auto node = m_scene.getSceneNodeCache().add( name ).lock();
		node->setPosition( doConvert( castor::Point2i{ cell.x, cell.y } )
			+ castor::Point3f{ 0, m_cellDimensions[1], 0 }
			+ castor::Point3f{ 0, base.getBoundingBox().getDimensions()[1], 0 }
			+ castor::Point3f{ 0, mesh.lock().get()->getBoundingBox().getDimensions()[1] / 2.0f, 0 } );
		node->attachTo( *m_mapNode );
		auto tower = m_scene.getGeometryCache().create( name, m_scene, *node, mesh );
		m_scene.getGeometryCache().add( tower );
		cell.state = GridCell::State::eTower;
		return tower;
	}

	castor3d::AnimatedObjectGroupSPtr Game::doCreateAnimation( castor3d::GeometrySPtr geometry
		, String const & animName
		, bool looped
		, bool paused )
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
		 animGroup->setAnimationLooped( animName, looped );
		 animGroup->startAnimation( animName );

		 if ( paused )
		 {
			 animGroup->pauseAnimation( animName );
		 }

		 return animGroup;
	 }

	 void Game::doSelectMapBlock( Entity const & entity )
	 {
		auto geometry = doGetGeometry( entity );

		if ( geometry )
		{
			m_hud.updateTowerInfo( m_ecs, Entity{} );
			m_hud.showBuild();
			m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePostRender
				, [this, geometry, entity]()
				{
					m_selectedBlock.select( entity, geometry );
					m_selectedEntity.unselect();
				} ) );
		}
	}

	 void Game::doSelectTower( Entity const & entity )
	{
		auto geometry = doGetGeometry( entity );

		if ( geometry )
		{
			m_hud.updateTowerInfo( m_ecs, entity );
			m_hud.hideBuild();
			m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePostRender
				, [this, geometry, entity]()
				{
					m_selectedBlock.unselect();
					m_selectedEntity.select( entity, geometry );
				} ) );
		}
	}

	 castor3d::GeometrySPtr Game::doGetGeometry( Entity const & entity )
	{
		 castor3d::GeometrySPtr result;

		try
		{
			result = m_ecs.getComponentData< castor3d::GeometrySPtr >( entity
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
