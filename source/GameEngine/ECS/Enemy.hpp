/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Enemy_HPP___
#define ___EFO_Enemy_HPP___

#include "GameEnginePrerequisites.hpp"

namespace orastus
{
	/**
	*\brief
	*	The components used to describe an enemy.
	*/
	class Enemy
	{
	public:
		/**
		*\brief
		*	Constructor, retrieves the components.
		*\param[in] p_ecs
		*	The ECS instance, from which the components are retrieved.
		*/
		EFO_API explicit Enemy( Ecs & p_ecs );
		/**
		*\brief
		*	Creates the component data for an enemy.
		*\param[in] p_entity
		*	The entity for the enemy.
		*\param[in] p_speed
		*	The speed value.
		*\param[in] p_life
		*	The life points.
		*\param[in] p_geometry
		*	The geometry value.
		*\param[in] p_walkData
		*	The wallk data.
		*/
		EFO_API void createData( Entity const & p_entity
			, float p_speed
			, uint32_t p_life
			, castor3d::GeometrySPtr p_geometry
			, WalkDataPtr p_walkData );
		/**
		*\brief
		*	Resets an enemy to initial state.
		*\param[in] p_entity
		*	The enemy entity.
		*\param[in] p_speed
		*	The speed value.
		*\param[in] p_life
		*	The life points.
		*\param[in] p_geometry
		*	The geometry value.
		*\param[in] p_walkData
		*	The wallk data.
		*/
		EFO_API void resetData( Entity const & p_entity
			, float p_speed
			, uint32_t p_life
			, castor3d::GeometrySPtr p_geometry
			, WalkDataPtr p_walkData );
		/**
		*\brief
		*	Returns an enemy as a string.
		*\param[in] p_entity
		*	The entity to print.
		*/
		EFO_API String toString( Entity const & p_entity );

	private:
		Ecs & m_ecs;
		Component const & m_speed;
		Component const & m_life;
		Component const & m_geometry;
		Component const & m_walk;
		Component const & m_state;
	};
}

#endif
