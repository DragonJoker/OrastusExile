/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_EnemySpawner_HPP___
#define ___EFO_EnemySpawner_HPP___

#include "Ability.hpp"

namespace orastus
{
	class EnemySpawner
	{
	public:
		EFO_API EnemySpawner( Ecs & p_ecs
			, Game & p_game );
		EFO_API ~EnemySpawner();

		EFO_API void Reset();
		EFO_API void Update( Milliseconds const & p_elapsed );

		EFO_API void KillEnemy( Entity p_enemy );

		inline uint32_t GetWave()const
		{
			return m_totalsWaves;
		}

		inline uint32_t GetEnemiesLife()const
		{
			return m_life.GetValue();
		}

		inline uint32_t GetEnemiesBounty()const
		{
			return m_bounty.GetValue();
		}

	private:
		void DoStartWave( uint32_t m_count );
		bool DoCanSpawn( std::chrono::milliseconds const & p_elapsed );
		void DoSpawn( Grid const & p_grid
			, GridPath const & p_path );

		inline bool DoIsWaveEnded()const
		{
			return m_count == 0;
		}

	private:
		Ecs & m_ecs;
		Game & m_game;
		uint32_t m_count{ 0 };
		std::chrono::milliseconds m_timeBetweenTwoSpawns{ 0u };
		std::chrono::milliseconds m_timeSinceLastSpawn{ 0u };
		uint32_t m_totalsWaves{ 0u };
		uint32_t m_totalSpawned{ 0u };
		uint32_t m_totalEnemies{ 0u };
		EntityList m_enemiesCache;
		EntityList m_liveEnemies;
		Ability< uint32_t > m_life;
		Ability< uint32_t > m_bounty;
	};
}

#endif
