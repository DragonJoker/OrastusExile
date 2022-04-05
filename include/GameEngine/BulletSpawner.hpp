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
		EFO_API BulletSpawner( Ecs & p_ecs
			, Game & p_game );
		EFO_API ~BulletSpawner();

		EFO_API void reset();
		EFO_API void fireBullet( Entity p_source
			, Entity p_target );
		EFO_API void fireBullet( Entity p_source
			, Entity p_target
			, castor3d::GeometrySPtr p_geometry );
		EFO_API void killBullet( Entity p_bullet );

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
