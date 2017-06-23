/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_MapBlock_HPP___
#define ___EFO_MapBlock_HPP___

#include "GameEnginePrerequisites.hpp"

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
		*\param[in] p_ecs
		*	The ECS instance, from which the components are retrieved.
		*/
		EFO_API explicit MapBlock( Ecs & p_ecs );
		/**
		*\brief
		*	Creates the component data for a map block.
		*\param[in] p_entity
		*	The entity for the map block.
		*\param[in] p_geometry
		*	The geometry value.
		*/
		EFO_API void CreateData( Entity const & p_entity
			, Castor3D::GeometrySPtr p_geometry );
		/**
		*\brief
		*	Returns a map block as a string.
		*\param[in] p_entity
		*	The entity to print.
		*/
		EFO_API String ToString( Entity const & p_entity );

	private:
		Ecs & m_ecs;
		Component const & m_geometry;
		Component const & m_pickable;
	};
}

#endif
