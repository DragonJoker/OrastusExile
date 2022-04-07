/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Ability_HPP___
#define ___EFO_Ability_HPP___

#include "GameEnginePrerequisites.hpp"

#include <functional>

namespace orastus
{
	template< typename T >
	struct Ability
	{
		using ValueIncrementFunction = std::function< T( T const &, uint32_t ) >;

		inline void initialise( T const & value
			, ValueIncrementFunction const & valueIncrement )
		{
			m_level = 0;
			m_value = value;
			m_valueIncrement = valueIncrement;
		}

		inline void upgrade()
		{
			m_value = m_valueIncrement( m_value, m_level );
			++m_level;
		}

		inline T const & getValue()const
		{
			return m_value;
		}

	protected:
		T m_value{};
		ValueIncrementFunction m_valueIncrement;
		uint32_t m_level{ 0u };
	};
}

#endif
