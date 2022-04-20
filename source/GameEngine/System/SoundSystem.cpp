/**
See licence file in root folder, MIT.txt
*/
#include "GameEngine/System/SoundSystem.hpp"

#include "GameEngine/Audio.hpp"
#include "GameEngine/Game.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/SoundSource.hpp"

namespace orastus
{
	SoundSystem::SoundSystem( Ecs & ecs )
		: m_ecs{ ecs }
	{
	}

	void SoundSystem::update( Game & game
		, Milliseconds const & elapsed )
	{
		game.getAudio().update( game.getCameraNode() );
		auto & sources = m_ecs.getComponentDatas( m_ecs.getComponent( Ecs::SoundSourceComponent ) );

		for ( auto & data : sources )
		{
			static_cast< ComponentData< SoundSource const * > & >( *data.data ).getValue()->update( game.getCameraNode() );
		}
	}
}
