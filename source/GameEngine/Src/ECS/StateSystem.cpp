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

	void StateSystem::Update( Milliseconds const & p_elapsed )
	{
		for ( auto & l_data : m_ecs.GetComponentDatas( m_ecs.GetComponent( Ecs::StateComponent ) ) )
		{
			static_cast< ComponentData< StateMachinePtr > & >( *l_data.m_data ).GetValue()->Update( p_elapsed );
		}
	}
}
