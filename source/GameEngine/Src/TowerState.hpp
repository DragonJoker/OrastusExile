/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_TowerState_HPP___
#define ___EFO_TowerState_HPP___

#include "GameEnginePrerequisites.hpp"

namespace orastus
{
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
	*\param[in] p_state
	*	The tower state.
	*\return
	*	The name.
	*/
	EFO_API String GetName( TowerState p_state );
}

#endif
