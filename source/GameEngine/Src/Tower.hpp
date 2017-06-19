/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Tower_HPP___
#define ___EFO_Tower_HPP___

#include "AbilitySet.hpp"

namespace orastus
{
	class Tower
	{
	public:
		enum class State
		{
			Idle,
			Spotted,
			Shooting,
		};

	public:
		inline Tower( AbilitySet const & p_abilities )
			: m_abilities{ p_abilities }
		{
		}

		inline State GetState()const
		{
			return m_state;
		}

		inline AbilitySet const & GetAbilities()const
		{
			return m_abilities;
		}

	private:
		State m_state{ State::Idle };
		AbilitySet m_abilities;
	};
}

#endif
