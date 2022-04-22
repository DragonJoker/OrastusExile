#include "GameEngine/Game.hpp"

#include "GameEngine/Audio.hpp"
#include "GameEngine/Ecs/Bullet.hpp"
#include "GameEngine/Ecs/Enemy.hpp"
#include "GameEngine/Ecs/SoundSource.hpp"
#include "GameEngine/Ecs/Tower.hpp"

#include <Castor3D/Engine.hpp>
#include <Castor3D/Event/Frame/FrameListener.hpp>
#include <Castor3D/Event/Frame/CpuFunctorEvent.hpp>
#include <Castor3D/Material/Material.hpp>
#include <Castor3D/Model/Mesh/Mesh.hpp>
#include <Castor3D/Model/Mesh/Submesh/Submesh.hpp>
#include <Castor3D/Model/Skeleton/Skeleton.hpp>
#include <Castor3D/Scene/Camera.hpp>
#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/Scene.hpp>
#include <Castor3D/Scene/Animation/AnimatedObject.hpp>
#include <Castor3D/Scene/Animation/AnimatedObjectGroup.hpp>
#include <Castor3D/Scene/Light/Light.hpp>
#include <Castor3D/Scene/Light/PointLight.hpp>

namespace orastus
{
	//*********************************************************************************************

	namespace game
	{
		int32_t constexpr targetXMin = -2;
		int32_t constexpr targetXMax = 2;
		int32_t constexpr targetYMin = -1;
		int32_t constexpr targetYMax = 3;
#if !defined( NDEBUG ) || Cheat
		uint32_t constexpr InitialGold = 75000u;
#else
		uint32_t constexpr InitialGold = 750u;
#endif

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

		void prepareGridLine( GridPathNode const & prv
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

		void prepareTargetsArea( GridPathNode const & cur
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

	castor3d::AnimatedObjectGroupSPtr createAnimation( castor3d::GeometrySPtr geometry
		, String const & animName
		, bool looped
		, bool paused )
	{
		CU_Require( geometry );
		auto & scene = *geometry->getScene();
		auto animGroup = scene.getAnimatedObjectGroupCache().add( geometry->getName(), scene ).lock();
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

	//*********************************************************************************************

	Game::Game( castor::Path const & dataFolder
		, castor3d::Scene & scene
		, castor3d::Camera const & camera )
		: m_audio{ castor::makeUnique< Audio >() }
		, m_scene{ scene }
		, m_camera{ camera }
		, m_hud{ *this, dataFolder, scene }
		, m_player{ *this, m_ecs, m_hud, game::InitialGold }
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
		, m_directTowerMesh{ m_scene.getMeshCache().find( cuT( "Ballista" ) ).lock() }
		, m_splashTowerMesh{ m_scene.getMeshCache().find( cuT( "Cannon" ) ).lock() }
		, m_arrowMesh{ m_scene.getMeshCache().find( cuT( "Arrow" ) ).lock() }
		, m_cannonBallMesh{ m_scene.getMeshCache().find( cuT( "CannonBall" ) ).lock() }
		, m_targetCapturedSounds{ &addSound( dataFolder / "Sounds" / "cow-1.wav", 10u )
			, &addSound( dataFolder / "Sounds" / "cow-2.wav", 10u )
			, &addSound( dataFolder / "Sounds" / "cow-3.wav", 10u )
			, &addSound( dataFolder / "Sounds" / "cow-4.wav", 10u ) }
		, m_ballistaShootSound{ addSound( dataFolder / "Sounds" / "ballista_shoot.wav", 20u ) }
		, m_cannonShootSound{ addSound( dataFolder / "Sounds" / "cannon_shoot.wav", 20u ) }
		, m_ballistaHitSound{ addSound( dataFolder / "Sounds" / "ballista_hit.wav", 20u ) }
		, m_cannonHitSound{ addSound( dataFolder / "Sounds" / "cannon_hit.wav", 20u ) }
		, m_enemyKillSound{ addSound( dataFolder / "Sounds" / "enemy_kill.wav", 20u ) }
		, m_towerBuildSound{ addSound( dataFolder / "Sounds" / "tower_build.wav", 1u ).createSource( false ) }
		, m_waveStartSound{ addSound( dataFolder / "Sounds" / "wave_start.wav", 1u ).createSource( false ) }
		, m_engine{ m_device() }
		, m_enemySpawner{ m_ecs, *this }
		, m_bulletSpawner{ m_ecs, *this }
		, m_targetSpawner{ m_ecs, *this }
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
		m_gold = game::InitialGold;
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

	void Game::killEnemy( Entity entity )
	{
		auto & enemy = m_ecs.getComponentData< EnemyData >( entity
			, m_ecs.getComponent( Ecs::EnemyStateComponent ) ).getValue();

		if ( enemy.target )
		{
			m_targetSpawner.freeTarget( enemy.target );
			enemy.target = {};
		}

		m_enemySpawner.killEnemy( entity );
	}

	void Game::enemyCapturing( EnemyData const & enemy )
	{
		if ( enemy.target )
		{
			m_targetSpawner.targetBeingCaptured( enemy.target );
		}
	}

	void Game::enemyEscaping( EnemyData const & enemy )
	{
		CU_Require( enemy.target );

		if ( m_targetSpawner.targetCaptured( enemy.target ) )
		{
			m_state = State::eEnded;
			m_hud.gameOver();
		}
	}

	void Game::enemyEscaped( EnemyData const & enemy )
	{
		m_enemySpawner.enemyEscaped( enemy );
	}

	void Game::killBullet( BulletData const & bullet )
	{
		m_bulletSpawner.killBullet( bullet );
	}

	void Game::hit( BulletData const & bullet
		, Entity target
		, uint32_t damage )
	{
		auto node = Game::getBulletNode( bullet.geometry );
		m_ecs.getComponentData< SoundSource const * >( bullet.entity
			, m_ecs.getComponent( Ecs::SoundSourceComponent ) ).getValue()->play( node );
		m_bulletSpawner.killBullet( bullet );

		auto & enemy = m_ecs.getComponentData< EnemyData >( target
			, m_ecs.getComponent( Ecs::EnemyStateComponent ) ).getValue();
		enemy.life -= std::min( enemy.life, damage );

		if ( !enemy.life )
		{
			killEnemy( target );
		}
	}

	void Game::areaDamage( BulletData const & bullet
		, Entity target
		, uint32_t damage )
	{
		auto node = Game::getBulletNode( bullet.geometry );
		auto position = node->getDerivedPosition();
		m_ecs.getComponentData< SoundSource const * >( bullet.entity
			, m_ecs.getComponent( Ecs::SoundSourceComponent ) ).getValue()->play( node );
		m_bulletSpawner.killBullet( bullet );
		auto area = 32.0f;
		auto & enemies = m_ecs.getComponentDatas( m_ecs.getComponent( Ecs::EnemyStateComponent ) );

		for ( auto & enemy : enemies )
		{
			if ( enemy.data )
			{
				auto & data = componentCast< EnemyData >( *enemy.data ).getValue();
				auto distance = castor::point::distance( getEnemyNode( data.geometry )->getDerivedPosition(), position );
				auto enemyRatio = distance / area;

				if ( enemyRatio <= 1.0 )
				{
					data.life -= std::min( data.life
						, uint32_t( float( damage ) * ( 1.0f - enemyRatio ) ) );

					if ( !data.life )
					{
						killEnemy( enemy.entity );
					}
				}
			}
		}
	}

	Entity Game::selectTarget()
	{
		return m_targetSpawner.selectTarget();
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

	void Game::createDirectTower()
	{
		if ( m_player.isEmptyCellSelected() )
		{
			auto & selectedCell = m_player.getSelectedCell();
			String name = cuT( "DirectTower_" ) + std::to_string( selectedCell.x ) + cuT( "x" ) + std::to_string( selectedCell.y );
			doCreateTowerBase( name + "Base"
				, selectedCell
				, m_towerBaseMesh );
			auto tower = doCreateTower( name
				, selectedCell
				, *m_towerBaseMesh.lock()
				, m_directTowerMesh );

			if ( tower )
			{
				m_towerBuildSound.play();
				selectedCell.entity = m_ecs.createTower( std::make_unique< DirectTower >()
					, selectedCell
					, tower
					, nullptr
					, std::make_unique< AttackData >( 0_ms )
					, &m_ballistaShootSound.createSource( *tower->getParent(), false ) );
				m_player.selectEntity( selectedCell.entity );
			}
		}
	}

	void Game::createSplashTower()
	{
		if ( m_player.isEmptyCellSelected() )
		{
			auto & selectedCell = m_player.getSelectedCell();
			String name = cuT( "SplashTower_" ) + std::to_string( selectedCell.x ) + cuT( "x" ) + std::to_string( selectedCell.y );
			doCreateTowerBase( name + "Base"
				, selectedCell
				, m_towerBaseMesh );
			auto tower = doCreateTower( name
				, selectedCell
				, *m_towerBaseMesh.lock()
				, m_splashTowerMesh );

			if ( tower )
			{
				m_towerBuildSound.play();
				selectedCell.entity = m_ecs.createTower( std::make_unique< SplashTower >()
					, selectedCell
					, tower
					, nullptr
					, std::make_unique< AttackData >( 0_ms )
					, &m_cannonShootSound.createSource( *tower->getParent(), false ) );
				m_player.selectEntity( selectedCell.entity );
			}
		}
	}

	void Game::createBullet( TowerData const & source
		, Entity target )
	{
		auto node = Game::getTowerNode( source.geometry );
		Sound * sound{};

		switch ( source.category->getType() )
		{
		case TowerType::eDirect:
			sound = &m_ballistaHitSound;
			break;
		case TowerType::eSplash:
			sound = &m_cannonHitSound;
			break;
		}

		m_ecs.getComponentData< SoundSource const * >( source.entity
			, m_ecs.getComponent( Ecs::SoundSourceComponent ) ).getValue()->play( node );

		if ( m_bulletSpawner.hasFreeBullet( source.category->getAmmoType() ) )
		{
			m_bulletSpawner.fireBullet( source
				, target
				, *sound );
		}
		else
		{
			m_bulletSpawner.fireBullet( source
				, target
				, *sound
				, doCreateAmmo( *node, source.category->getAmmoType() ) );
		}
	}

	castor::Point3f Game::convert( castor::Point2i const & position )const
	{
		return castor::Point3f( float( position[0] - int( m_grid.getWidth() ) / 2 ) * m_cellDimensions[0]
			, 0
			, float( position[1] - int( m_grid.getHeight() ) / 2 ) * m_cellDimensions[2] );
	}

	Sound & Game::getTargetCapturedSound()
	{
		std::uniform_int_distribution< size_t > dist( 0u, m_targetCapturedSounds.size() - 1u );
		return *m_targetCapturedSounds[dist( m_engine )];
	}

	Sound & Game::addSound( castor::Path const & file
		, uint32_t maxSources )
	{
		return m_audio->addSound( Sound::Type::eSfx, file, maxSources );
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

	castor3d::SceneNodeRPtr Game::getTargetNode( castor3d::GeometrySPtr geometry )
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
				game::prepareGridLine( *prv, *cur, from, m_grid );
				++prv;
				++cur;
			}

			game::prepareTargetsArea( *prv, m_scene, m_grid );
		}
	}

	void Game::doAddTile( GridCell & cell
		, castor3d::MeshResPtr mesh
		, castor::Quaternion const & orientation
		, bool pickable )
	{
		String name = cuT( "Tile_" ) + std::to_string( cell.x ) + cuT( "x" ) + std::to_string( cell.y );

		if ( !m_scene.getSceneNodeCache().has( name ) )
		{
			auto node = m_scene.getSceneNodeCache().add( name ).lock();
			auto geometry = m_scene.getGeometryCache().create( name, m_scene, *node, mesh );
			node->setPosition( doConvert( castor::Point2i{ cell.x, cell.y } )
				+ castor::Point3f{ 0, m_cellDimensions[1] / 2, 0 } );
			node->setOrientation( orientation );
			node->attachTo( *m_mapNode );
			m_scene.getGeometryCache().add( geometry );
			cell.entity = m_ecs.createMapBlock( cell, geometry, pickable );
		}
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
		for ( uint32_t x = cell.x + game::targetXMin; x <= cell.x + game::targetXMax; ++x )
		{
			for ( uint32_t y = cell.y + game::targetYMin; y <= cell.y + game::targetYMax; ++y )
			{
				m_targetSpawner.spawn( m_grid( y, x ) );
			}
		}
	}

	castor3d::MeshResPtr Game::doGetAmmoMesh( AmmoType type )
	{
		switch ( type )
		{
		case orastus::AmmoType::eDirect:
			return m_arrowMesh;
		case orastus::AmmoType::eSplash:
			return m_cannonBallMesh;
		default:
			return castor3d::MeshResPtr{};
		}
	}

	castor3d::GeometrySPtr Game::doCreateAmmo( castor3d::SceneNode const & origin
		, AmmoType type )
	{
		String name = getName( type ) + cuT( "Ammo_" ) + toString( m_bulletSpawner.getBulletsCount() );
		auto node = m_scene.getSceneNodeCache().add( name ).lock();
		auto geometry = m_scene.getGeometryCache().create( name
			, m_scene
			, *node
			, doGetAmmoMesh( type ) );
		node->setPosition( origin.getDerivedPosition() );
		node->attachTo( *m_mapNode );
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

	castor::Point3f Game::doConvert( castor::Point2i const & position )const
	{
		return castor::Point3f( float( position[0] - int( m_grid.getWidth() ) / 2 ) * m_cellDimensions[0]
			, 0
			, float( position[1] - int( m_grid.getHeight() ) / 2 ) * m_cellDimensions[2] );
	}
}
