#include "GameEngine/EnemySpawner.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/WalkData.hpp"

#include <Castor3D/Model/Mesh/Mesh.hpp>
#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/Scene.hpp>
#include <Castor3D/Scene/SceneNode.hpp>
#include <Castor3D/Scene/Light/PointLight.hpp>

namespace orastus
{
	EnemySpawner::EnemySpawner( Ecs & ecs
		, Game & game )
		: m_ecs{ ecs }
		, m_game{ game }
	{
		auto & scene = m_game.getScene();
		m_enemyMeshes.push_back( scene.getMeshCache().find( cuT( "UfoGreen" ) ).lock() );
		m_enemyMeshes.push_back( scene.getMeshCache().find( cuT( "UfoPurple" ) ).lock() );
		m_enemyMeshes.push_back( scene.getMeshCache().find( cuT( "UfoRed" ) ).lock() );
		m_enemyMeshes.push_back( scene.getMeshCache().find( cuT( "UfoYellow" ) ).lock() );
		reset();
	}

	EnemySpawner::~EnemySpawner()
	{
	}

	void EnemySpawner::reset()
	{
		if ( m_enemiesCache )
		{
			m_enemiesCache->insert( m_enemiesCache->end()
				, m_liveEnemies.begin()
				, m_liveEnemies.end() );
		}

		m_liveEnemies.clear();

		for ( auto & enemies : m_allEnemiesCache )
		{
			for ( auto & enemy : enemies.second )
			{
				auto geometry = m_ecs.getComponentData< castor3d::GeometrySPtr >( enemy
					, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
				Game::getEnemyNode( geometry )->setPosition( castor::Point3f{ 0, -1000, 0 } );
			}
		}

		m_life.initialise( 1u
			, []( uint32_t value, uint32_t level )
			{
				return value + std::max( level, uint32_t( value * 5.0 / 100.0 ) );
			} );

		m_bounty.initialise( 9u
			, []( uint32_t value, uint32_t level )
			{
				return value + std::max( 2u, ( value * 4 ) / 100 );
			} );

		m_totalsWaves = 0u;
		m_totalEnemies = 0u;
		m_timeSinceLastSpawn = std::chrono::milliseconds{};
		m_count = 0u;
		m_kills = 0u;
		m_enemiesCache = nullptr;
	}

	void EnemySpawner::update( Milliseconds const & elapsed )
	{
		if ( m_liveEnemies.empty()
			&& !m_count )
		{
			//m_game.gain( m_spawner.getWave() * 2 );
#if !defined( NDEBUG )
			doStartWave( 2u );
#else
			doStartWave( 10u );
#endif
		}

		if ( doCanSpawn( elapsed ) )
		{
			doSpawn( m_game.getGrid(), m_game.getPath() );
		}
	}

	void EnemySpawner::killEnemy( Entity enemy )
	{
		auto it = std::find( std::begin( m_liveEnemies )
			, std::end( m_liveEnemies )
			, enemy );

		if ( it != std::end( m_liveEnemies ) )
		{
			m_liveEnemies.erase( it );
			++m_kills;
		}

		m_enemiesCache->push_back( enemy );
		auto geometry = m_ecs.getComponentData< castor3d::GeometrySPtr >( enemy
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
		Game::getEnemyNode( geometry )->setPosition( castor::Point3f{ 0, -1000, 0 } );
	}

	void EnemySpawner::doStartWave( uint32_t count )
	{
		m_count = count;
		++m_totalsWaves;
		m_timeBetweenTwoSpawns = std::chrono::milliseconds( 1000 );
		m_timeSinceLastSpawn = m_timeBetweenTwoSpawns;
		m_life.upgrade();
		m_bounty.upgrade();
		auto index = getWave() % uint32_t( m_enemyMeshes.size() );
		m_currentEnemies = m_enemyMeshes[index];
		m_enemiesCache = &m_allEnemiesCache.emplace( m_currentEnemies.lock().get(), EntityList{} ).first->second;
		std::cout << "\nStarting wave " << m_totalsWaves << std::endl;
	}

	bool EnemySpawner::doCanSpawn( std::chrono::milliseconds const & elapsed )
	{
		m_timeSinceLastSpawn += elapsed;
		return m_count && m_timeSinceLastSpawn >= m_timeBetweenTwoSpawns;
	}

	void EnemySpawner::doSpawn( Grid const & grid
		, GridPath const & path )
	{
		auto & pathNode = *path.begin();
		auto & cell = grid( pathNode.y, pathNode.x );

		if ( m_enemiesCache->empty() )
		{
			String name = cuT( "Enemy_" ) + std::to_string( ++m_totalSpawned );
			auto geometry = m_game.createEnemy( name, m_currentEnemies );
			Game::getEnemyNode( geometry )->setPosition( m_game.convert( castor::Point2i{ cell.x, cell.y - 1 } )
				+ castor::Point3f{ 0, m_game.getCellHeight() / 2.0f, 0 } );
			m_liveEnemies.push_back( m_ecs.createEnemy( 24.0f
				, m_life.getValue()
				, geometry
				, std::make_shared< WalkData >( path, m_game ) ) );
		}
		else
		{
			auto entity = m_enemiesCache->front();
			m_liveEnemies.push_back( entity );
			m_enemiesCache->erase( m_enemiesCache->begin() );
			auto geometry = m_ecs.getComponentData< castor3d::GeometrySPtr >( entity
				, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			Game::getEnemyNode( geometry )->setPosition( m_game.convert( castor::Point2i{ cell.x, cell.y - 1 } )
				+ castor::Point3f{ 0, m_game.getCellHeight() / 2.0f, 0 } );
			m_ecs.resetEnemy( entity
				, 24.0f
				, m_life.getValue()
				, geometry
				, std::make_shared< WalkData >( path, m_game ) );
		}

		--m_count;
		m_timeSinceLastSpawn = std::chrono::milliseconds{};
		++m_totalEnemies;
	}
}
