/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_StateSystem_HPP___
#define ___EFO_StateSystem_HPP___

#include "GameEngine/GameEnginePrerequisites.hpp"

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
		*\param[in] kind
		*	The resource kind.
		*\param[in] count
		*	The resource count.
		*/
		EFO_API StateSystem( Ecs & ecs );
		/**
		*\brief
		*	Updates the states.
		*\param[in] game
		*	The game instance.
		*\param[in] elapsed
		*	The time elapsed since last call.
		*/
		EFO_API void update( Game & game
			, Milliseconds const & elapsed );

	private:
		Ecs & m_ecs;
	};
}

#endif
