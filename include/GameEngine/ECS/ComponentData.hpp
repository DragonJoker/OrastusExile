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
	using ComponentDataPtr = std::shared_ptr< BaseComponentData >;
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
		inline explicit ComponentData( T const & p_value )
			: m_value{ p_value }
		{
		}
		/**
		*\return
		*	The component data's value.
		*/
		inline T const & getValue()const
		{
			return m_value;
		}
		/**
		*\brief
		*	Sets the component data's value.
		*\param[in] p_value
		*	The new value.
		*/
		inline void setValue( T const & p_value )
		{
			m_value = p_value;
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
		return p_lhs.getValue() == p_rhs.getValue();
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
	std::shared_ptr< ComponentData< T > > makeComponentData( T const & p_value )
	{
		return std::make_shared< ComponentData< T > >( p_value );
	}
	/**
	*\brief
	*	Casts from one component data type to another.
	*\param[in] p_value
	*	The component data.
	*\return
	*	The casted ComponentData.
	*/
	template< typename T >
	inline ComponentData< T > & componentCast( BaseComponentData & p_data )
	{
#if !defined( NDEBUG )

		if ( !dynamic_cast< ComponentData< T > const * >( &p_data ) )
		{
			CU_Failure( "ComponentData type mismatch." );
		}

#endif

		return static_cast< ComponentData< T > & >( p_data );
	}
	/**
	*\brief
	*	Casts from one component data type to another.
	*\param[in] p_value
	*	The component data.
	*\return
	*	The casted ComponentData.
	*/
	template< typename T >
	inline ComponentData< T > const & componentCast( BaseComponentData const & p_data )
	{
#if !defined( NDEBUG )

		if ( !dynamic_cast< ComponentData< T > const * >( &p_data ) )
		{
			CU_Failure( "ComponentData type mismatch." );
		}

#endif

		return static_cast< ComponentData< T > const & >( p_data );
	}
}

#endif
