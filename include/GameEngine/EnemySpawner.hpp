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
		EFO_API EnemySpawner( Ecs & ecs
			, Game & game );
		EFO_API ~EnemySpawner();

		EFO_API void reset();
		EFO_API void update( Milliseconds const & elapsed );
		/**
		*\brief
		*	Kills an enemy.
		*\param[in] entity
		*	The enemy entity.
		*/
		EFO_API void killEnemy( Entity enemy );
		/**
		*\brief
		*	An enemy has escaped.
		*\param[in] entity
		*	The enemy entity.
		*/
		EFO_API void enemyEscaped( Entity enemy );

		uint32_t getWave()const
		{
			return m_totalsWaves;
		}

		uint32_t getEnemiesLife()const
		{
			return m_life.getValue();
		}

		uint32_t getEnemiesBounty()const
		{
			return m_bounty.getValue();
		}

		uint32_t getKills()const
		{
			return m_kills;
		}

	private:
		void doStartWave( uint32_t count );
		bool doCanSpawn( std::chrono::milliseconds const & elapsed );
		void doSpawn( Grid const & grid
			, GridPath const & path );

		bool doIsWaveEnded()const
		{
			return m_count == 0;
		}

	private:
		Ecs & m_ecs;
		Game & m_game;
		uint32_t m_count{ 0 };
		uint32_t m_kills{ 0 };
		std::chrono::milliseconds m_timeBetweenTwoSpawns{ 0u };
		std::chrono::milliseconds m_timeSinceLastSpawn{ 0u };
		uint32_t m_totalsWaves{ 0u };
		uint32_t m_totalSpawned{ 0u };
		uint32_t m_totalEnemies{ 0u };
		std::vector< castor3d::MeshResPtr > m_enemyMeshes;
		std::map< castor3d::MeshRPtr, EntityList > m_allEnemiesCache;
		EntityList * m_enemiesCache{};
		EntityList m_liveEnemies;
		castor3d::MeshResPtr m_currentEnemies;
		Ability< uint32_t > m_life;
		Ability< uint32_t > m_bounty;
	};
}

#endif
