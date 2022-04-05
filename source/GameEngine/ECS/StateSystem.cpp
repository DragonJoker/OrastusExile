/**
See licence file in root folder, MIT.txt
*/
#include "GameEngine/ECS/StateSystem.hpp"

#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/State/StateMachine.hpp"

namespace orastus
{
	StateSystem::StateSystem( Ecs & p_ecs )
		: m_ecs{ p_ecs }
	{
	}

	void StateSystem::update( Game & p_game
		, Milliseconds const & p_elapsed )
	{
		auto l_states = m_ecs.getComponentDatas( m_ecs.getComponent( Ecs::StateComponent ) );
		for ( auto & l_data : l_states )
		{
			static_cast< ComponentData< StateMachinePtr > & >( *l_data.data ).getValue()->update( p_game
				, p_elapsed );
		}
	}
}
