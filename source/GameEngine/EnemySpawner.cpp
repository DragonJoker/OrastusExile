#include "EnemySpawner.hpp"

#include "Game.hpp"
#include "ECS/Ecs.hpp"
#include "ECS/WalkData.hpp"

#include <Castor3D/Model/Mesh/Mesh.hpp>
#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/Scene.hpp>
#include <Castor3D/Scene/SceneNode.hpp>
#include <Castor3D/Scene/Light/PointLight.hpp>

using namespace castor;
using namespace castor3d;

namespace orastus
{
	EnemySpawner::EnemySpawner( Ecs & p_ecs
		, Game & p_game )
		: m_ecs{ p_ecs }
		, m_game{ p_game }
	{
		reset();
	}

	EnemySpawner::~EnemySpawner()
	{
	}

	void EnemySpawner::reset()
	{
		m_enemiesCache.insert( m_enemiesCache.end()
			, m_liveEnemies.begin()
			, m_liveEnemies.end() );
		m_liveEnemies.clear();

		for ( auto & l_enemy : m_enemiesCache )
		{
			auto l_geometry = m_ecs.getComponentData< GeometrySPtr >( l_enemy
				, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			Game::getEnemyNode( l_geometry )->setPosition( Point3f{ 0, -1000, 0 } );
		}

		m_life.initialise( 1u, []( uint32_t p_value, uint32_t p_level )
		{
			return p_value + std::max( p_level, uint32_t( p_value * 5.0 / 100.0 ) );
		} );

		m_bounty.initialise( 9u, []( uint32_t p_value, uint32_t p_level )
		{
			return p_value + std::max( 2u, ( p_value * 4 ) / 100 );
		} );

		m_totalsWaves = 0u;
		m_totalEnemies = 0u;
		m_timeSinceLastSpawn = std::chrono::milliseconds{};
		m_count = 0u;
	}

	void EnemySpawner::update( Milliseconds const & p_elapsed )
	{
		if ( m_liveEnemies.empty()
			&& !m_count )
		{
			//m_game.Gain( m_spawner.getWave() * 2 );
#if !defined( NDEBUG )
			doStartWave( 2u );
#else
			doStartWave( 10u );
#endif
		}

		if ( doCanSpawn( p_elapsed ) )
		{
			doSpawn( m_game.getGrid(), m_game.getPath() );
		}
	}

	void EnemySpawner::killEnemy( Entity p_enemy )
	{
		auto l_it = std::find( std::begin( m_liveEnemies )
			, std::end( m_liveEnemies )
			, p_enemy );

		if ( l_it != std::end( m_liveEnemies ) )
		{
			m_liveEnemies.erase( l_it );
		}

		m_enemiesCache.push_back( p_enemy );
		auto l_geometry = m_ecs.getComponentData< GeometrySPtr >( p_enemy
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
		Game::getEnemyNode( l_geometry )->setPosition( Point3f{ 0, -1000, 0 } );
	}

	void EnemySpawner::doStartWave( uint32_t p_count )
	{
		m_count = p_count;
		++m_totalsWaves;
		m_timeBetweenTwoSpawns = std::chrono::milliseconds( 1000 );
		m_timeSinceLastSpawn = m_timeBetweenTwoSpawns;
		m_life.upgrade();
		m_bounty.upgrade();
		std::cout << "\nStarting wave " << m_totalsWaves << std::endl;
	}

	bool EnemySpawner::doCanSpawn( std::chrono::milliseconds const & p_elapsed )
	{
		m_timeSinceLastSpawn += p_elapsed;
		return m_count && m_timeSinceLastSpawn >= m_timeBetweenTwoSpawns;
	}

	void EnemySpawner::doSpawn( Grid const & p_grid
		, GridPath const & p_path )
	{
		Entity l_result;
		GeometrySPtr l_geometry;
		auto & l_pathNode = *p_path.begin();
		auto & l_cell = p_grid( l_pathNode.m_y, l_pathNode.m_x );

		if ( m_enemiesCache.empty() )
		{
			String l_name = cuT( "EnemyCube_" ) + std::to_string( ++m_totalSpawned );
			l_geometry = m_game.createEnemy( l_name );
			Game::getEnemyNode( l_geometry )->setPosition( m_game.convert( Point2i{ l_cell.m_x, l_cell.m_y - 1 } )
				+ Point3f{ 0, m_game.getCellHeight(), 0 } );
			m_liveEnemies.push_back( m_ecs.createEnemy( 24.0f
				, m_life.getValue()
				, l_geometry
				, std::make_shared< WalkData >( p_path, m_game ) ) );
		}
		else
		{
			l_result = m_enemiesCache.front();
			m_liveEnemies.push_back( l_result );
			m_enemiesCache.erase( m_enemiesCache.begin() );
			l_geometry = m_ecs.getComponentData< GeometrySPtr >( l_result
				, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			Game::getEnemyNode( l_geometry )->setPosition( m_game.convert( Point2i{ l_cell.m_x, l_cell.m_y - 1 } )
				+ Point3f{ 0, m_game.getCellHeight(), 0 } );
			m_ecs.resetEnemy( l_result
				, 24.0f
				, m_life.getValue()
				, l_geometry
				, std::make_shared< WalkData >( p_path, m_game ) );
		}

		--m_count;
		m_timeSinceLastSpawn = std::chrono::milliseconds{};
		++m_totalEnemies;
	}
}
