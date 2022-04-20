#include "GameEngine/State/TargetState.hpp"

#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/Entity.hpp"
#include "GameEngine/ECS/WalkData.hpp"
#include "GameEngine/Game.hpp"

#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/SceneNode.hpp>

namespace orastus
{
	String getName( TargetState state )
	{
		switch ( state )
		{
		case TargetState::eIdle:
			return cuT( "Idle" );
		case TargetState::eCapturing:
			return cuT( "Capturing" );
		case TargetState::eCaptured:
			return cuT( "Captured" );
		default:
			CU_Failure( "Unexpected target state." );
			return "Unknown";
		}
	}

	namespace target
	{
		namespace details
		{
			bool capture( Game & game
				, Ecs & ecs
				, Entity const & target
				, castor3d::SceneNode & node
				, Milliseconds const & elapsed )
			{
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

		State createIdleState( Ecs & ecs
			, Entity const & entity )
		{
			return State{ [&ecs, entity]( Game & game
				, Milliseconds const & elapsed
				, Milliseconds const & total )
				{
					auto state = ecs.getComponentData< TargetState >( entity
						, ecs.getComponent( Ecs::StatusComponent ) ).getValue();
					return state != TargetState::eIdle;
				} };
		}

		State createCapturingState( Ecs & ecs
			, Entity const & entity )
		{
			auto geometry = ecs.getComponentData< castor3d::GeometrySPtr >( entity
				, ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			auto node = Game::getTargetNode( geometry );

			return State{ [&ecs, entity, node]( Game & game
				, Milliseconds const & elapsed
				, Milliseconds const & total )
			{
				auto state = ecs.getComponentData< TargetState >( entity
					, ecs.getComponent( Ecs::StatusComponent ) ).getValue();
				bool result = state == TargetState::eCaptured
					|| state == TargetState::eIdle;

				if ( !result )
				{
					auto const angle = castor::Angle::fromDegrees( -float( elapsed.count() ) * 240 / 1000.0f );
					node->yaw( angle );
					result = details::capture( game
						, ecs
						, entity
						, *node
						, elapsed );
				}

				return result;
			} };
		}
	}
}
