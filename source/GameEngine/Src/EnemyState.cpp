#include "EnemyState.hpp"

namespace orastus
{
	String GetName( EnemyState p_state )
	{
		String l_result;

		switch ( p_state )
		{
		case EnemyState::eWalking:
			l_result = cuT( "Walking" );
			break;

		case EnemyState::eFrozen:
			l_result = cuT( "Frozen" );
			break;

		case EnemyState::eSlown:
			l_result = cuT( "Slown" );
			break;

		case EnemyState::eDying:
			l_result = cuT( "Dying" );
			break;

		case EnemyState::eDead:
			l_result = cuT( "Dead" );
			break;

		case EnemyState::eArrived:
			l_result = cuT( "Arrived" );
			break;

		default:
			FAILURE( "Unexpected enemy state." );
			break;
		}

		return l_result;
	}
}
