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
		*\param[in] p_name
		*	The optional entity name, used in debug only.
		*/
		inline Entity( xchar const * const p_name ) 
#if !defined( NDEBUG )
			: m_name{ p_name }
#endif
		{
		}

	private:
#if !defined( NDEBUG )
		String m_name;
#endif
	};
	//! An entity list.
	using EntityList = std::vector< Entity >;
}

#endif
