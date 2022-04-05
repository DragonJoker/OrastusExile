#include "GameEngine/State/State.hpp"

namespace orastus
{
	State::State( Function p_function )
		: m_function{ p_function }
		, m_elapsed{ 0_ms }
	{
	}

	bool State::run( Game & p_game
		, Milliseconds const & p_elapsed )
	{
		m_elapsed += p_elapsed;
		auto l_result = m_function( p_game
			, p_elapsed
			, m_elapsed );

		if ( l_result )
		{
			m_elapsed = 0_ms;
		}

		return l_result;
	}
}
