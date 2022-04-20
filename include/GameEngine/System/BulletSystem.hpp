/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_BulletSystem_HPP___
#define ___EFO_BulletSystem_HPP___

#include "GameEngine/GameEnginePrerequisites.hpp"

namespace orastus
{
	/**
	*\brief
	*	The ECS system that updates the bullets.
	*/
	class BulletSystem
	{
	public:
		/**@}*/
		/**
		*\brief
		*	Constructor.
		*\param[in] count
		*	The resource count.
		*/
		EFO_API BulletSystem( Ecs & ecs );
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
