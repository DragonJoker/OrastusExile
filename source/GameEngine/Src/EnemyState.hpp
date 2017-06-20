/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_EnemyState_HPP___
#define ___EFO_EnemyState_HPP___

#include "GameEnginePrerequisites.hpp"

namespace orastus
{
	/**
	*\brief
	*	Enemy states enumeration.
	*/
	enum class EnemyState
	{
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
		//! The enemy is arrived to its target.
		eArrived,
	};
	/**
	*\brief
	*	Retrieves the name of the given enemy state.
	*\param[in] p_state
	*	The tower state.
	*\return
	*	The name.
	*/
	EFO_API String GetName( EnemyState p_state );
}

#endif
