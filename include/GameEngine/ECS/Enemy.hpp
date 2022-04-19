/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Enemy_HPP___
#define ___EFO_Enemy_HPP___

#include "GameEngine/GameEnginePrerequisites.hpp"

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
		*\param[in] ecs
		*	The ECS instance, from which the components are retrieved.
		*/
		EFO_API explicit Enemy( Ecs & ecs );
		/**
		*\brief
		*	Creates the component data for an enemy.
		*\param[in] entity
		*	The entity for the enemy.
		*\param[in] speed
		*	The speed value.
		*\param[in] life
		*	The life points.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] walkData
		*	The walk data.
		*\param[in] killSound
		*	The sound emitted when the enemy is killed.
		*/
		EFO_API void createData( Entity const & entity
			, float speed
			, uint32_t life
			, castor3d::GeometrySPtr geometry
			, WalkDataPtr walkData
			, SoundSource killSound );
		/**
		*\brief
		*	Resets an enemy to initial state.
		*\param[in] entity
		*	The enemy entity.
		*\param[in] speed
		*	The speed value.
		*\param[in] life
		*	The life points.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] walkData
		*	The wallk data.
		*/
		EFO_API void resetData( Entity const & entity
			, float speed
			, uint32_t life
			, castor3d::GeometrySPtr geometry
			, WalkDataPtr walkData );
		/**
		*\brief
		*	Returns an enemy as a string.
		*\param[in] entity
		*	The entity to print.
		*/
		EFO_API String toString( Entity const & entity );

	private:
		Ecs & m_ecs;
		Component const & m_speed;
		Component const & m_life;
		Component const & m_cooldown;
		Component const & m_target;
		Component const & m_geometry;
		Component const & m_walk;
		Component const & m_state;
		Component const & m_status;
		Component const & m_killSound;
	};
}

#endif
