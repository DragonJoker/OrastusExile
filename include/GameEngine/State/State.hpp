/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_State_HPP___
#define ___EFO_State_HPP___

#include "GameEngine/GameEnginePrerequisites.hpp"

#include <functional>

namespace orastus
{
	/**
	*\brief
	*	State base class.
	*/
	class State
	{
	public:
		using Function = std::function< bool( Game & game
			, Milliseconds const & elapsed
			, Milliseconds const & total ) >;

	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] function
		*	The state function to execute at each update.
		*/
		State( Function function );
		/**
		*\brief
		*	Runs the action for the state.
		*\param[in] elapsed
		*	The time elapsed since last call.
		*\return
		*	\p true if the state is ended.
		*/
		bool run( Game & game
			, Milliseconds const & elapsed );

	private:
		Function m_function;
		Milliseconds m_elapsed;
	};
}

#endif
