/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_BulletSpawner_HPP___
#define ___EFO_BulletSpawner_HPP___

#include "Ability.hpp"

namespace orastus
{
	class BulletSpawner
	{
	public:
		EFO_API BulletSpawner( Ecs & ecs
			, Game & game );
		EFO_API ~BulletSpawner();

		EFO_API void reset();
		EFO_API void fireBullet( TowerData const & source
			, Entity target
			, Sound & sound );
		EFO_API void fireBullet( TowerData const & source
			, Entity target
			, Sound & sound
			, castor3d::GeometryRPtr geometry );
		EFO_API void killBullet( BulletData const & bullet );

		inline bool hasFreeBullet( AmmoType ammo )const
		{
			return !m_bulletsCaches[size_t( ammo )].empty();
		}

		inline uint32_t getBulletsCount()const
		{
			return m_totalSpawned;
		}

	private:
		Ecs & m_ecs;
		Game & m_game;
		uint32_t m_totalSpawned{ 0u };
		std::array< EntityList, size_t( AmmoType::eCount ) > m_bulletsCaches;
		std::array< EntityList, size_t( AmmoType::eCount ) > m_liveBullets;
	};
}

#endif
