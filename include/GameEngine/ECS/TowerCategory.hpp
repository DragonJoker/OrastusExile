/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_TowerCategory_HPP___
#define ___EFO_TowerCategory_HPP___

#include "GameEngine/GameEnginePrerequisites.hpp"

namespace orastus
{
	struct TowerCategory
	{
		/**
		*\brief
		*	Constructor.
		*\param[in] type
		*	The tower type.
		*\param[in] damage
		*	The damage value.
		*\param[in] cooldown
		*	The cooldown value.
		*\param[in] range
		*	The range value.
		*\param[in] bulletSpeed
		*	The bullet speed value.
		*/
		TowerCategory( TowerType type
			, uint32_t damage
			, Milliseconds cooldown
			, float range
			, float bulletSpeed )
			: m_type{ type }
			, m_damage{ damage }
			, m_cooldown{ cooldown }
			, m_range{ range }
			, m_bulletSpeed{ bulletSpeed }
		{
		}

		EFO_API virtual ~TowerCategory() = default;

		uint32_t getDamage()const
		{
			return m_damage;
		}

		float getRange()const
		{
			return m_range;
		}

		Milliseconds getCooldown()const
		{
			return m_cooldown;
		}

		float getBulletSpeed()const
		{
			return m_bulletSpeed;
		}

		TowerType getType()const
		{
			return m_type;
		}

	protected:
		TowerType m_type;
		uint32_t m_damage;
		Milliseconds m_cooldown;
		float m_range;
		float m_bulletSpeed;
	};

	struct DirectTower
		: public TowerCategory
	{
		EFO_API DirectTower();

		static constexpr uint32_t Damage = 3u;
		static constexpr Milliseconds Cooldown = 700_ms;
		static constexpr float Range = 40.0f;
		static constexpr float BulletSpeed = 240.0f;
	};

	struct SplashTower
		: public TowerCategory
	{
		EFO_API SplashTower();

		static constexpr uint32_t Damage = 5u;
		static constexpr Milliseconds Cooldown = 2000_ms;
		static constexpr float Range = 100.0f;
		static constexpr float BulletSpeed = 200.0f;
	};
}

#endif
