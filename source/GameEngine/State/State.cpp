#include "GameEngine/State/State.hpp"

namespace orastus
{
	State::State( Function function )
		: m_function{ function }
		, m_elapsed{ 0_ms }
	{
	}

	bool State::run( Game & game
		, Milliseconds const & elapsed )
	{
		m_elapsed += elapsed;
		auto result = m_function( game
			, elapsed
			, m_elapsed );

		if ( result )
		{
			m_elapsed = 0_ms;
		}

		return result;
	}
}
