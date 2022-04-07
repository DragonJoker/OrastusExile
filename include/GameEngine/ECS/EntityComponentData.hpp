/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_EntityComponentData_HPP___
#define ___EFO_EntityComponentData_HPP___

#include "ComponentData.hpp"
#include "Entity.hpp"

#include <vector>

namespace orastus
{
	/**
	*\brief
	*	Contains the component data linked to a component and an entity.
	*/
	struct EntityComponentData
	{
		/**
		*\brief
		*	Constructor.
		*\param[in] p_entity
		*	The entity.
		*\param[in] p_data
		*	The data.
		*/
		inline EntityComponentData( Entity const & p_entity
			, ComponentDataPtr && p_data )
			: entity{ p_entity }
			, data{ std::move( p_data ) }
		{
		}

		//! The entity's ID.
		Entity entity;
		//! The component's data.
		ComponentDataPtr data;
	};
	//! A list of EntityComponentData
	using EntityComponentsList = std::vector< EntityComponentData >;
	//! A map of EntityComponentsList, sorted by component ID
	using EntityComponentsMap = std::map< ComponentId, EntityComponentsList >;
}

#endif