/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Enemy_HPP___
#define ___EFO_Enemy_HPP___

#include "EnemyState.hpp"

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
		EFO_API Enemy( Ecs & p_ecs );
		/**
		*\brief
		*	Creates the component data for an enemy.
		*\param[in] p_entity
		*	The entity for the enemy.
		*\param[in] p_speed
		*	The speed value.
		*\param[in] p_life
		*	The life points.
		*/
		void CreateData( Entity const & p_entity
			, float p_speed
			, uint32_t p_life );

	private:
		Ecs & m_ecs;
		Component const & m_speed;
		Component const & m_life;
		Component const & m_state;
	};
}

#endif
