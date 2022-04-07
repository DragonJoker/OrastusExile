#include "GameEngine/State/StateMachine.hpp"

namespace orastus
{
	StateMachine::StateMachine( State const & initial
		, bool looped )
		: m_looped{ looped }
	{
		m_states.push_back( initial );
		m_current = std::begin( m_states );
	}

	void StateMachine::addState( State const & state )
	{
		if ( m_current != std::end( m_states ) )
		{
			auto offset = std::distance( std::begin( m_states ), m_current );
			m_states.push_back( state );
			m_current = std::begin( m_states ) + offset;
		}
		else
		{
			m_states.push_back( state );
			m_current = std::rbegin( m_states ).base();
		}
	}

	void StateMachine::update( Game & game
		, Milliseconds const & elapsed )
	{
		if ( m_current != std::end( m_states ) )
		{
			auto & current = *m_current;

			if ( current.run( game, elapsed ) )
			{
				++m_current;
			}
		}

		if ( m_current == std::end( m_states )
			&& m_looped )
		{
			m_current = std::begin( m_states );
		}
	}

	void StateMachine::restart()
	{
		m_current = std::begin( m_states );
	}
}
