/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_StateMachine_HPP___
#define ___EFO_StateMachine_HPP___

#include "State.hpp"

namespace orastus
{
	/**
	*\brief
	*	State machine.
	*/
	class StateMachine
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] p_initial
		*	The initial state.
		*\param[in] p_looped
		*	Tells if the state machine should start again to the inital state
		*	after reaching the end (\p true) or not.
		*/
		StateMachine( State const & p_initial
			, bool p_looped );
		/**
		*\brief
		*	Adds a state to the list.
		*\param[in] p_state
		*	The state.
		*/
		void addState( State const & p_state );
		/**
		*\brief
		*	Updates the machine.
		*	Runs the current state, and swithes to the next one if needed.
		*\param[in] p_elapsed
		*	The time elapsed since last call.
		*/
		void update( Game & p_game
			, Milliseconds const & p_elapsed );
		/**
		*\brief
		*	Restarts the machine.
		*/
		void restart();

	private:
		StateArray::iterator m_current;
		StateArray m_states;
		bool m_looped;
	};
}

#endif
