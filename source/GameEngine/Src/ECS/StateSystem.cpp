/**
See licence file in root folder, MIT.txt
*/
#include "StateSystem.hpp"

#include "ECS/Ecs.hpp"
#include "State/StateMachine.hpp"

namespace orastus
{
	StateSystem::StateSystem( Ecs & p_ecs )
		: m_ecs{ p_ecs }
	{
	}

	void StateSystem::Update( Game & p_game
		, Milliseconds const & p_elapsed )
	{
		auto l_states = m_ecs.GetComponentDatas( m_ecs.GetComponent( Ecs::StateComponent ) );
		for ( auto & l_data : l_states )
		{
			static_cast< ComponentData< StateMachinePtr > & >( *l_data.m_data ).GetValue()->Update( p_game
				, p_elapsed );
		}
	}
}
