/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_EnemyState_HPP___
#define ___EFO_EnemyState_HPP___

#include "State.hpp"

namespace orastus
{
	namespace enemy
	{
		/**
		*\brief
		*	Creates the spawning state for given enemy.
		*\param[in] ecs
		*	The ECS instance.
		*\param[in] entity
		*	The enemy's entity.
		*/
		EFO_API State createSpawningState( Ecs & ecs, Entity const & entity );
		/**
		*\brief
		*	Creates the walking state for given enemy.
		*\param[in] ecs
		*	The ECS instance.
		*\param[in] entity
		*	The enemy's entity.
		*/
		EFO_API State createWalkingState( Ecs & ecs, Entity const & entity );
		/**
		*\brief
		*	Creates the escaping state for given enemy.
		*\param[in] ecs
		*	The ECS instance.
		*\param[in] entity
		*	The enemy's entity.
		*/
		EFO_API State createEscapingState( Ecs & ecs, Entity const & entity );
	}
	/**
	*\brief
	*	Enemy states enumeration.
	*/
	enum class EnemyState
	{
		//! The enemy is spawning.
		eSpawning,
		//! The enemy is walking.
		eWalking,
		//! The enemy is frozen.
		eFrozen,
		//! The enemy is slown down.
		eSlown,
		//! The enemy is dying (id est: playing the death animation)
		eDying,
		//! The enemy is dead.
		eDead,
		//! The enemy is capturing a target.
		eCapturing,
		//! The enemy is escaping the map.
		eEscaping,
		//! The enemy has escaped the map.
		eEscaped,
	};
	/**
	*\brief
	*	Retrieves the name of the given enemy state.
	*\param[in] state
	*	The tower state.
	*\return
	*	The name.
	*/
	EFO_API String getName( EnemyState state );
}

#endif
