/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_StateSystem_HPP___
#define ___EFO_StateSystem_HPP___

#include "GameEnginePrerequisites.hpp"

namespace orastus
{
	/**
	*\brief
	*	The ECS system that updates the states.
	*/
	class StateSystem
	{
	public:
		/**@}*/
		/**
		*\brief
		*	Constructor.
		*\param[in] p_kind
		*	The resource kind.
		*\param[in] p_count
		*	The resource count.
		*/
		EFO_API StateSystem( Ecs & p_ecs );
		/**
		*\brief
		*	Updates the states.
		*\param[in] p_game
		*	The game instance.
		*\param[in] p_elapsed
		*	The time elapsed since last call.
		*/
		EFO_API void Update( Game & p_game
			, Milliseconds const & p_elapsed );

	private:
		Ecs & m_ecs;
	};
}

#endif
