#include "EnemySpawner.hpp"

#include "Game.hpp"
#include "ECS/Ecs.hpp"
#include "ECS/WalkData.hpp"

#include <Mesh/Mesh.hpp>
#include <Scene/Geometry.hpp>
#include <Scene/Scene.hpp>
#include <Scene/SceneNode.hpp>
#include <Scene/Light/PointLight.hpp>

using namespace Castor;
using namespace Castor3D;

namespace orastus
{
	EnemySpawner::EnemySpawner( Ecs & p_ecs
		, Game & p_game )
		: m_ecs{ p_ecs }
		, m_game{ p_game }
	{
		Reset();
	}

	EnemySpawner::~EnemySpawner()
	{
	}

	void EnemySpawner::Reset()
	{
		m_enemiesCache.insert( m_enemiesCache.end()
			, m_liveEnemies.begin()
			, m_liveEnemies.end() );
		m_liveEnemies.clear();

		for ( auto & l_enemy : m_enemiesCache )
		{
			auto l_geometry = m_ecs.GetComponentData< GeometrySPtr >( l_enemy
				, m_ecs.GetComponent( Ecs::GeometryComponent ) ).GetValue();
			l_geometry->GetParent()->GetParent()->SetPosition( Point3r{ 0, -1000, 0 } );
		}

		m_life.Initialise( 1u, []( uint32_t p_value, uint32_t p_level )
		{
			return p_value + std::max( p_level, uint32_t( p_value * 5.0 / 100.0 ) );
		} );

		m_bounty.Initialise( 9u, []( uint32_t p_value, uint32_t p_level )
		{
			return p_value + std::max( 2u, ( p_value * 4 ) / 100 );
		} );

		m_totalsWaves = 0u;
		m_totalEnemies = 0u;
		m_timeSinceLastSpawn = std::chrono::milliseconds{};
		m_count = 0u;
	}

	void EnemySpawner::Update( Milliseconds const & p_elapsed )
	{
		if ( m_liveEnemies.empty() && DoIsWaveEnded() )
		{
			//m_game.Gain( m_spawner.GetWave() * 2 );
#if !defined( NDEBUG )
			DoStartWave( 2u );
#else
			DoStartWave( 10u );
#endif
		}

		if ( DoCanSpawn( p_elapsed ) )
		{
			DoSpawn( m_game.GetGrid(), m_game.GetPath() );
		}
	}

	void EnemySpawner::KillEnemy( Entity p_enemy )
	{
		auto l_geometry = m_ecs.GetComponentData< GeometrySPtr >( p_enemy
			, m_ecs.GetComponent( Ecs::GeometryComponent ) ).GetValue();
		l_geometry->GetParent()->GetParent()->SetPosition( Point3r{ 0, -1000, 0 } );
		m_enemiesCache.push_back( p_enemy );
	}

	void EnemySpawner::DoStartWave( uint32_t p_count )
	{
		m_count = p_count;
		++m_totalsWaves;
		m_timeBetweenTwoSpawns = std::chrono::milliseconds( 1000 );
		m_timeSinceLastSpawn = m_timeBetweenTwoSpawns;
	}

	bool EnemySpawner::DoCanSpawn( std::chrono::milliseconds const & p_elapsed )
	{
		m_timeSinceLastSpawn += p_elapsed;
		return m_count && m_timeSinceLastSpawn >= m_timeBetweenTwoSpawns;
	}

	void EnemySpawner::DoSpawn( Grid const & p_grid
		, GridPath const & p_path )
	{
		Entity l_result;
		GeometrySPtr l_geometry;
		auto & l_pathNode = *p_path.begin();
		auto & l_cell = p_grid( l_pathNode.m_y, l_pathNode.m_x );

		if ( m_enemiesCache.empty() )
		{
			String l_name = cuT( "EnemyCube_" ) + std::to_string( ++m_totalSpawned );
			l_geometry = m_game.CreateEnemy( l_name );
			m_liveEnemies.push_back( m_ecs.CreateEnemy( 24.0f
				, m_life.GetValue()
				, l_geometry
				, std::make_shared< WalkData >( p_path, m_game ) ) );
		}
		else
		{
			l_result = m_enemiesCache.front();
			m_enemiesCache.erase( m_enemiesCache.begin() );
			l_geometry = m_ecs.GetComponentData< GeometrySPtr >( l_result
				, m_ecs.GetComponent( Ecs::GeometryComponent ) ).GetValue();
			m_ecs.ResetEnemy( l_result
				, 24.0f
				, m_life.GetValue()
				, l_geometry
				, std::make_shared< WalkData >( p_path, m_game ) );
		}

		l_geometry->GetParent()->GetParent()->SetPosition( m_game.Convert( Point2i{ l_cell.m_x, l_cell.m_y - 1 } ) + Point3r{ 0, m_game.GetCellHeight(), 0 } );
		--m_count;
		m_timeSinceLastSpawn = std::chrono::milliseconds{};
		++m_totalEnemies;
	}
}
