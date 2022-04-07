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
		*\param[in] initial
		*	The initial state.
		*\param[in] looped
		*	Tells if the state machine should start again to the inital state
		*	after reaching the end (\p true) or not.
		*/
		StateMachine( State const & initial
			, bool looped );
		/**
		*\brief
		*	Adds a state to the list.
		*\param[in] state
		*	The state.
		*/
		void addState( State const & state );
		/**
		*\brief
		*	Updates the machine.
		*	Runs the current state, and swithes to the next one if needed.
		*\param[in] elapsed
		*	The time elapsed since last call.
		*/
		void update( Game & game
			, Milliseconds const & elapsed );
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
