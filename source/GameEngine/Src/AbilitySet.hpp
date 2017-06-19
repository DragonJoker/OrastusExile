/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_AbilitySet_HPP___
#define ___EFO_AbilitySet_HPP___

#include "Ability.hpp"

namespace orastus
{
	class AbilitySet
	{
	public:
		inline AbilitySet( String const & p_name
			, CooldownAbility const & p_cooldown
			, DamageAbility const & p_damage
			, RangeAbility const & p_range
			, SpeedAbility const & p_bulletSpeed
			, uint32_t p_level )
			: m_name{ p_name }
			, m_cooldown{ p_cooldown }
			, m_damage{ p_damage }
			, m_range{ p_range }
			, m_bulletSpeed{ p_bulletSpeed }
			, m_level{ p_level }
		{
		}

		inline AbilitySet( String const & p_name
			, CooldownAbility const & p_cooldown
			, DamageAbility const & p_damage
			, RangeAbility const & p_range
			, SpeedAbility const & p_bulletSpeed
			, DamageAbility const & p_splashDamage
			, RangeAbility const & p_splashRange
			, uint32_t p_level )
			: m_name{ p_name }
			, m_cooldown{ p_cooldown }
			, m_damage{ p_damage }
			, m_range{ p_range }
			, m_bulletSpeed{ p_bulletSpeed }
			, m_level{ p_level }
			, m_splashDamage{ p_splashDamage }
			, m_splashRange{ p_splashRange }
		{
		}

		inline Milliseconds const & GetCooldown()const
		{
			return m_cooldown.GetValue();
		}

		inline uint32_t GetDamage()const
		{
			return m_damage.GetValue();
		}

		inline float GetRange()const
		{
			return m_range.GetValue();
		}

		inline float GetBulletSpeed()const
		{
			return m_bulletSpeed.GetValue();
		}

		inline uint32_t GetSplashDamage()const
		{
			return m_splashDamage.GetValue();
		}

		inline float GetSplashRange()const
		{
			return m_splashRange.GetValue();
		}

		inline uint32_t GetRequiredLevel()const
		{
			return m_level;
		}

		inline String const & GetName()const
		{
			return m_name;
		}

	protected:
		String m_name;
		CooldownAbility m_cooldown;
		DamageAbility m_damage;
		RangeAbility m_range;
		SpeedAbility m_bulletSpeed;
		DamageAbility m_splashDamage{ 0u };
		RangeAbility m_splashRange{ 0.0f };
		uint32_t m_level;
	};
}

#endif
