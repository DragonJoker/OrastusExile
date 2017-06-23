#include "StateMachine.hpp"

namespace orastus
{
	StateMachine::StateMachine( State const & p_initial
		, bool p_looped )
		: m_looped{ p_looped }
	{
		m_states.push_back( p_initial );
		m_current = std::begin( m_states );
	}

	void StateMachine::AddState( State const & p_state )
	{
		if ( m_current != std::end( m_states ) )
		{
			auto l_offset = std::distance( std::begin( m_states ), m_current );
			m_states.push_back( p_state );
			m_current = std::begin( m_states ) + l_offset;
		}
		else
		{
			m_states.push_back( p_state );
			m_current = std::rbegin( m_states ).base();
		}
	}

	void StateMachine::Update( Game & p_game
		, Milliseconds const & p_elapsed )
	{
		if ( m_current != std::end( m_states ) )
		{
			auto & l_current = *m_current;

			if ( l_current.Run( p_game, p_elapsed ) )
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

	void StateMachine::Restart()
	{
		m_current = std::begin( m_states );
	}
}
