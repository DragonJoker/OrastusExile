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

		EFO_API void Reset();
		EFO_API void FireBullet( Entity p_source
			, Entity p_target );
		EFO_API void FireBullet( Entity p_source
			, Entity p_target
			, Castor3D::GeometrySPtr p_geometry );
		EFO_API void KillBullet( Entity p_bullet );

		inline bool HasFreeBullet()const
		{
			return !m_bulletsCache.empty();
		}

		inline uint32_t GetBulletsCount()const
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
