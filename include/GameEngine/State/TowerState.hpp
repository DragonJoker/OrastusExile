/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_TowerState_HPP___
#define ___EFO_TowerState_HPP___

#include "State.hpp"

namespace orastus
{
	namespace tower
	{
		/**
		*\brief
		*	Creates the idle state for given tower.
		*\param[in] ecs
		*	The ECS instance.
		*\param[in] tower
		*	The tower's entity.
		*/
		EFO_API State createIdleState( Ecs & ecs, Entity const & entity );
		/**
		*\brief
		*	Creates the shooting state for given tower.
		*\param[in] ecs
		*	The ECS instance.
		*\param[in] tower
		*	The tower's entity.
		*/
		EFO_API State createShootingState( Ecs & ecs, Entity const & entity );
	}
	/**
	*\brief
	*	Tower states enumeration.
	*/
	enum class TowerState
	{
		//! No executor for the tower.
		eEmpty,
		//! The tower is looking for an enemy to shoot at.
		eIdle,
		//! The tower has spotted an enemy.
		eSpotted,
		//! The tower is shooting (id est: it is playing the shooting animation).
		eShooting,
	};
	/**
	*\brief
	*	Retrieves the name of the given tower state.
	*\param[in] state
	*	The tower state.
	*\return
	*	The name.
	*/
	EFO_API String getName( TowerState state );
}

#endif
