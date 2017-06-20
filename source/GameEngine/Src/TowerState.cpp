#include "TowerState.hpp"

namespace orastus
{
	String GetName( TowerState p_state )
	{
		String l_result;

		switch ( p_state )
		{
		case TowerState::eEmpty:
			l_result = cuT( "Empty" );
			break;

		case TowerState::eIdle:
			l_result = cuT( "Idle" );
			break;

		case TowerState::eSpotted:
			l_result = cuT( "Spotted" );
			break;

		case TowerState::eShooting:
			l_result = cuT( "Shooting" );
			break;

		default:
			FAILURE( "Unexpected tower state." );
			break;
		}

		return l_result;
	}
}
