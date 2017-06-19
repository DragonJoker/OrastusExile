/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Ability_HPP___
#define ___EFO_Ability_HPP___

#include "GameEnginePrerequisites.hpp"

namespace orastus
{
	template< typename T >
	class Ability
	{
	public:
		inline Ability( T const & p_value )
			: m_value{ p_value }
		{
		}

		inline T const & GetValue()const
		{
			return m_value;
		}

	protected:
		T m_value{};
	};

	template< typename T, typename U >
	inline bool operator==( Ability< T > const & p_lhs, Ability< U > const & p_rhs )
	{
		return false;
	}

	template< typename T >
	inline bool operator==( Ability< T > const & p_lhs, Ability< T > const & p_rhs )
	{
		return p_lhs.GetValue() == p_rhs.GetValue();
	}

	template< typename T, typename U >
	inline bool operator!=( Ability< T > const & p_lhs, Ability< U > const & p_rhs )
	{
		return !( p_lhs == p_rhs );
	}

	using CooldownAbility = Ability< Milliseconds >;
	using DamageAbility = Ability< uint32_t >;
	using RangeAbility = Ability< float >;
	using SpeedAbility = Ability< float >;
}

#endif
