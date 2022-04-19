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
		EFO_API void fireBullet( Entity source
			, Entity target
			, Sound const & sound );
		EFO_API void fireBullet( Entity source
			, Entity target
			, Sound const & sound
			, castor3d::GeometrySPtr geometry );
		EFO_API void killBullet( Entity bullet );

		inline bool hasFreeBullet()const
		{
			return !m_bulletsCache.empty();
		}

		inline uint32_t getBulletsCount()const
		{
			return m_totalSpawned;
		}

	private:
		Ecs & m_ecs;
		Game & m_game;
		uint32_t m_totalSpawned{ 0u };
		EntityList m_bulletsCache;
		EntityList m_liveBullets;
	};
}

#endif
