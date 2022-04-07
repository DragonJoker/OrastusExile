/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_EnemyState_HPP___
#define ___EFO_EnemyState_HPP___

#include "State.hpp"

namespace orastus
{
	namespace bullet
	{
		/**
		*\brief
		*	Creates the tracking state for given bullet.
		*\param[in] ecs
		*	The ECS instance.
		*\param[in] entity
		*	The bullet's entity.
		*/
		EFO_API State createTrackingState( Ecs & ecs, Entity const & entity );
	}
}

#endif
