/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_State_HPP___
#define ___EFO_State_HPP___

#include "GameEnginePrerequisites.hpp"

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
		using Function = std::function< bool( Milliseconds const & p_elapsed, Milliseconds const & p_total ) >;

	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] p_function
		*	The state function to execute at each update.
		*/
		State( Function p_function );
		/**
		*\brief
		*	Runs the action for the state.
		*\param[in] p_elapsed
		*	The time elapsed since last call.
		*\return
		*	\p true if the state is ended.
		*/
		bool Run( Milliseconds const & p_elapsed );

	private:
		Function m_function;
		Milliseconds m_elapsed;
	};
}

#endif
