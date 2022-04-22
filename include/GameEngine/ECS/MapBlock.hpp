/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_MapBlock_HPP___
#define ___EFO_MapBlock_HPP___

#include "GameEngine/GameEnginePrerequisites.hpp"

namespace orastus
{
	/**
	*\brief
	*	The components used to describe a map block.
	*/
	class MapBlock
	{
	public:
		/**
		*\brief
		*	Constructor, retrieves the components.
		*\param[in] ecs
		*	The ECS instance, from which the components are retrieved.
		*/
		EFO_API explicit MapBlock( Ecs & ecs );
		/**
		*\brief
		*	Creates the component data for a map block.
		*\param[in] entity
		*	The entity for the map block.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] pickable
		*	\p true to make the entity pickable.
		*/
		EFO_API void createData( Entity const & entity
			, GridCell & cell
			, castor3d::GeometrySPtr geometry
			, bool pickable );
		/**
		*\brief
		*	Returns a map block as a string.
		*\param[in] entity
		*	The entity to print.
		*/
		EFO_API String toString( Entity const & entity );

	private:
		Ecs & m_ecs;
		Component const & m_cell;
		Component const & m_geometry;
		Component const & m_pickable;
	};
}

#endif
