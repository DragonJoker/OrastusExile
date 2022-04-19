/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_TargetState_HPP___
#define ___EFO_TargetState_HPP___

#include "State.hpp"

namespace orastus
{
	namespace target
	{
		/**
		*\brief
		*	Creates the idle state for given enemies target.
		*\param[in] ecs
		*	The ECS instance.
		*\param[in] entity
		*	The enemy's entity.
		*/
		EFO_API State createIdleState( Ecs & ecs, Entity const & entity );
		/**
		*\brief
		*	Creates the "being captured" state for given enemies target.
		*\param[in] ecs
		*	The ECS instance.
		*\param[in] entity
		*	The enemy's entity.
		*/
		EFO_API State createCapturingState( Ecs & ecs, Entity const & entity );
	}
	/**
	*\brief
	*	Target states enumeration.
	*/
	enum class TargetState
	{
		//! The target is idle.
		eIdle,
		//! The target is being captured.
		eCapturing,
		//! The target is captured.
		eCaptured,
	};
	/**
	*\brief
	*	Retrieves the name of the given enemy state.
	*\param[in] state
	*	The tower state.
	*\return
	*	The name.
	*/
	EFO_API String getName( TargetState state );
}

#endif
