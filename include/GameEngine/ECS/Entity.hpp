/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Entity_HPP___
#define ___EFO_Entity_HPP___

#include "Id.hpp"

namespace orastus
{
	/**
	*\brief
	*	The most simple representation of an entity, for an ECS.
	*/
	class Entity
		: public Ided
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] name
		*	The optional entity name, used in debug only.
		*/
		inline explicit Entity( xchar const * const name ) 
#if !defined( NDEBUG )
			: m_name{ name }
#endif
		{
		}
		/**
		*\brief
		*	Constructor, does not generate the ID.
		*/
		inline Entity() 
			: Ided{ NoInit{} }
		{
		}
		/**
		*\return
		*	\p true if the id is generated.
		*/
		inline explicit operator bool()const
		{
			return Ided::operator bool();
		}

	private:
#if !defined( NDEBUG )
		String m_name;
#endif
	};
	/**
	*\brief
	*	Equality operator.
	*\param[in] lhs, rhs
	*	The operands.
	*\return
	*	\p false if \p lhs and \p rhs are different.
	*/
	bool inline operator==( Entity const & lhs, Entity const & rhs )
	{
		return static_cast< Ided >( lhs ) == static_cast< Ided >( rhs );
	}
}

#endif
