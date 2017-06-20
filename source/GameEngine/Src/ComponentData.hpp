/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_ComponentData_HPP___
#define ___EFO_ComponentData_HPP___

#include "Component.hpp"

#include <memory>

namespace orastus
{
	/**
	*\brief
	*	Base class for component data.
	*/
	class BaseComponentData
	{
	public:
		virtual ~BaseComponentData() = default;
	};
	//! A pointer to a BaseComponentData.
	using ComponentDataPtr = std::unique_ptr< BaseComponentData >;
	//! A list of ComponentData pointers.
	using ComponentDataList = std::vector< ComponentDataPtr >;
	/**
	*\brief
	*	Template component data.
	*/
	template< typename T >
	class ComponentData
		: public BaseComponentData
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] p_value
		*	The component data's value.
		*/
		inline ComponentData( T const & p_value )
			: m_value{ p_value }
		{
		}
		/**
		*\return
		*	The component data's value.
		*/
		inline T const & GetValue()const
		{
			return m_value;
		}

	protected:
		T m_value{};
	};
	/**
	*\name Logic operators.
	*/
	/**@{*/
	template< typename T, typename U >
	inline bool operator==( ComponentData< T > const & p_lhs, ComponentData< U > const & p_rhs )
	{
		return false;
	}

	template< typename T >
	inline bool operator==( ComponentData< T > const & p_lhs, ComponentData< T > const & p_rhs )
	{
		return p_lhs.GetValue() == p_rhs.GetValue();
	}

	template< typename T, typename U >
	inline bool operator!=( ComponentData< T > const & p_lhs, ComponentData< U > const & p_rhs )
	{
		return !( p_lhs == p_rhs );
	}
	/**@}*/
	/**
	*\brief
	*	Convenience function to create an entity component data.
	*\param[in] p_value
	*	The component data's value.
	*\return
	*	The created ComponentData.
	*/
	template< typename T >
	std::unique_ptr< ComponentData< T > > MakeComponentData( T const & p_value )
	{
		return std::make_unique< ComponentData< T > >( p_value );
	}
}

#endif
