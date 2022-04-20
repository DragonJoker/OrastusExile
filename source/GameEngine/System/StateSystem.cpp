/**
See licence file in root folder, MIT.txt
*/
#include "GameEngine/System/StateSystem.hpp"

#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/State/StateMachine.hpp"

namespace orastus
{
	StateSystem::StateSystem( Ecs & ecs )
		: m_ecs{ ecs }
	{
	}

	void StateSystem::update( Game & game
		, Milliseconds const & elapsed )
	{
		auto & states = m_ecs.getComponentDatas( m_ecs.getComponent( Ecs::StateComponent ) );

		for ( auto & data : states )
		{
			if ( data.data )
			{
				auto & machine = static_cast< ComponentData< StateMachinePtr > & >( *data.data ).getValue();

				if ( machine )
				{
					machine->update( game
						, elapsed );
				}
			}
		}
	}
}