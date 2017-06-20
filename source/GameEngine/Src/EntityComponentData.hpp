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
		*\param[in] p_component
		*	The component.
		*\param[in] p_data
		*	The data.
		*/
		inline EntityComponentData( Entity const & p_entity
			, Component const & p_component
			, ComponentDataPtr && p_data )
			: m_entity{ p_entity.GetId() }
			, m_component{ p_component.GetId() }
			, m_data{ std::move( p_data ) }
		{
		}

		//! The entity's ID.
		Id m_entity;
		//! The component's ID.
		Id m_component;
		//! The component's data.
		ComponentDataPtr m_data;
	};
	//! A list of EntityComponentData
	using EntityComponentsList = std::vector< EntityComponentData >;
}

#endif
