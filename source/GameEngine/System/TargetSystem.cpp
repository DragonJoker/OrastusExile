#include "GameEngine/System/TargetSystem.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/Entity.hpp"
#include "GameEngine/ECS/Target.hpp"

#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/SceneNode.hpp>

namespace orastus
{
	String getName( TargetStatus state )
	{
		switch ( state )
		{
		case TargetStatus::eIdle:
			return cuT( "Idle" );
		case TargetStatus::eCapturing:
			return cuT( "Capturing" );
		case TargetStatus::eCaptured:
			return cuT( "Captured" );
		default:
			CU_Failure( "Unexpected target state." );
			return "Unknown";
		}
	}

	namespace target
	{
		bool capture( Game & game
			, Ecs & ecs
			, Milliseconds const & elapsed
			, TargetData & target )
		{
			auto & node = *Game::getTargetNode( target.geometry );
			auto const angle = castor::Angle::fromDegrees( -float( elapsed.count() ) * 240 / 1000.0f );
			node.yaw( angle );
			castor::Point3f startPosition = castor::Point3f{ 0, game.getCellHeight() / 5.0f, 0 };
			castor::Point3f nextPosition = castor::Point3f{ 0, game.getCellHeight() * 2.0f, 0 };
			auto speed = float( castor::point::length( nextPosition - startPosition ) ) / 2.0f;
			auto distance = float( elapsed.count() ) * speed / 1000.0f;
			castor::Point3f position{ node.getPosition() };
			castor::Point3f direction{ nextPosition - castor::Point3f{ 0.0f, position->y, 0.0f } };
			auto distanceToDst = float( castor::point::length( direction ) );
			direction[1] *= distance / distanceToDst;
			bool result{ distanceToDst <= distance };

			if ( !result )
			{
				// not at destination
				nextPosition = position + direction;
				node.setPosition( nextPosition );
			}

			return result;
		}
	}

	TargetSystem::TargetSystem( Ecs & ecs )
		: m_ecs{ ecs }
	{
	}

	void TargetSystem::update( Game & game
		, Milliseconds const & elapsed )
	{
		auto & states = m_ecs.getComponentDatas( m_ecs.getComponent( Ecs::TargetStateComponent ) );

		for ( auto & data : states )
		{
			if ( data.data )
			{
				auto & target = static_cast< ComponentData< TargetData > & >( *data.data ).getValue();

				switch ( target.status )
				{
				case TargetStatus::eIdle:
					break;
				case TargetStatus::eCapturing:
					target::capture( game, m_ecs, elapsed, target );
					break;
				case TargetStatus::eCaptured:
					break;
				default:
					break;
				}
			}
		}
	}
}
