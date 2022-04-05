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

		EFO_API void reset();
		EFO_API void update( Milliseconds const & p_elapsed );

		EFO_API void killEnemy( Entity p_enemy );

		inline uint32_t getWave()const
		{
			return m_totalsWaves;
		}

		inline uint32_t getEnemiesLife()const
		{
			return m_life.getValue();
		}

		inline uint32_t getEnemiesBounty()const
		{
			return m_bounty.getValue();
		}

	private:
		void doStartWave( uint32_t m_count );
		bool doCanSpawn( std::chrono::milliseconds const & p_elapsed );
		void doSpawn( Grid const & p_grid
			, GridPath const & p_path );

		inline bool doIsWaveEnded()const
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
