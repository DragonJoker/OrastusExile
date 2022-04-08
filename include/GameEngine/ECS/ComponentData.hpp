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
		*\param[in] value
		*	The component data's value.
		*/
		inline explicit ComponentData( T const & value )
			: m_value{ value }
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
		*\param[in] value
		*	The new value.
		*/
		inline void setValue( T const & value )
		{
			m_value = value;
		}

	protected:
		T m_value{};
	};
	/**
	*\name Logic operators.
	*/
	/**@{*/
	template< typename T, typename U >
	inline bool operator==( ComponentData< T > const & lhs, ComponentData< U > const & rhs )
	{
		return false;
	}

	template< typename T >
	inline bool operator==( ComponentData< T > const & lhs, ComponentData< T > const & rhs )
	{
		return lhs.getValue() == rhs.getValue();
	}

	template< typename T, typename U >
	inline bool operator!=( ComponentData< T > const & lhs, ComponentData< U > const & rhs )
	{
		return !( lhs == rhs );
	}
	/**@}*/
	/**
	*\brief
	*	Convenience function to create an entity component data.
	*\param[in] value
	*	The component data's value.
	*\return
	*	The created ComponentData.
	*/
	template< typename T >
	std::shared_ptr< ComponentData< T > > makeComponentData( T const & value )
	{
		return std::make_shared< ComponentData< T > >( value );
	}
	/**
	*\brief
	*	Casts from one component data type to another.
	*\param[in] value
	*	The component data.
	*\return
	*	The casted ComponentData.
	*/
	template< typename T >
	inline ComponentData< T > & componentCast( BaseComponentData & data )
	{
#if !defined( NDEBUG )

		if ( !dynamic_cast< ComponentData< T > const * >( &data ) )
		{
			CU_Failure( "ComponentData type mismatch." );
		}

#endif

		return static_cast< ComponentData< T > & >( data );
	}
	/**
	*\brief
	*	Casts from one component data type to another.
	*\param[in] value
	*	The component data.
	*\return
	*	The casted ComponentData.
	*/
	template< typename T >
	inline ComponentData< T > const & componentCast( BaseComponentData const & data )
	{
#if !defined( NDEBUG )

		if ( !dynamic_cast< ComponentData< T > const * >( &data ) )
		{
			CU_Failure( "ComponentData type mismatch." );
		}

#endif

		return static_cast< ComponentData< T > const & >( data );
	}
}

#endif
