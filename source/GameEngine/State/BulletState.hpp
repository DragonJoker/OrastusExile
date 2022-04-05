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
		*\param[in] p_ecs
		*	The ECS instance.
		*\param[in] p_entity
		*	The bullet's entity.
		*/
		EFO_API State createTrackingState( Ecs & p_ecs, Entity const & p_entity );
	}
}

#endif
