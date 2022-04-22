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

		Ability( T value
			, ValueIncrementFunction valueIncrement )
			: m_value{ std::move( value ) }
			, m_valueIncrement{ std::move( valueIncrement ) }
		{
		}

		void initialise( T value
			, ValueIncrementFunction valueIncrement )
		{
			m_level = 0;
			m_value = std::move( value );
			m_valueIncrement = std::move( valueIncrement );
		}

		void upgrade()
		{
			m_value = m_valueIncrement( m_value, m_level );
			++m_level;
		}

		T const & getValue()const
		{
			return m_value;
		}

	protected:
		T m_value{};
		ValueIncrementFunction m_valueIncrement;
		uint32_t m_level{ 0u };
	};

	template< typename T >
	struct PaidAbility
		: private Ability< T >
	{
		using ValueIncrementFunction = typename Ability< T >::ValueIncrementFunction;
		using CostIncrementFunction = std::function< uint32_t( uint32_t, uint32_t ) >;

		PaidAbility( T value
			, ValueIncrementFunction valueIncrement
			, uint32_t cost
			, CostIncrementFunction costIncrement
			, uint32_t maxLevel )
			: Ability< T >{ std::move( value ), std::move( valueIncrement ) }
			, m_cost{ std::move( cost ) }
			, m_costIncrement{ std::move( costIncrement ) }
			, m_maxLevel{ std::move( maxLevel ) }
		{
		}

		void initialise( T const & value
			, ValueIncrementFunction const & valueIncrement
			, uint32_t cost
			, CostIncrementFunction costIncrement
			, uint32_t maxLevel = 15u )
		{
			Ability< T >::initialise( std::move( value ), std::move( valueIncrement ) );
			m_cost = std::move( cost );
			m_costIncrement = std::move( costIncrement );
			m_maxLevel = std::move( maxLevel );
		}

		void upgrade()
		{
			if ( canUpgrade() )
			{
				m_cost = m_costIncrement( m_cost, Ability< T >::m_level );
				Ability< T >::upgrade();
			}
		}

		bool canUpgrade()
		{
			return Ability< T >::m_level < m_maxLevel;
		}

		uint32_t const & getCost()const
		{
			return m_cost;
		}

		using Ability< T >::getValue;

	protected:
		uint32_t m_cost{ 0u };
		CostIncrementFunction m_costIncrement;
		uint32_t m_maxLevel{ 15u };
	};
}

#endif
