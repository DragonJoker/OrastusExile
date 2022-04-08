#include "GameEngine/State/EnemyState.hpp"

#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/Entity.hpp"
#include "GameEngine/ECS/WalkData.hpp"
#include "GameEngine/Game.hpp"

#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/SceneNode.hpp>

namespace orastus
{
	String getName( EnemyState state )
	{
		switch ( state )
		{
		case EnemyState::eWalking:
			return cuT( "Walking" );
		case EnemyState::eFrozen:
			return cuT( "Frozen" );
		case EnemyState::eSlown:
			return cuT( "Slown" );
		case EnemyState::eDying:
			return cuT( "Dying" );
		case EnemyState::eDead:
			return cuT( "Dead" );
		case EnemyState::eArrived:
			return cuT( "Arrived" );
		default:
			CU_Failure( "Unexpected enemy state." );
			return "Unknown";
		}
	}

	namespace enemy
	{
		bool walkToDestination( Game & game
			, WalkData & data
			, float speed
			, castor3d::SceneNode & node
			, Milliseconds const & elapsed )
		{
			auto distance = float( elapsed.count() ) * speed / 1000.0f;
			castor::Point3f nextPosition = data.destination;
			castor::Point3f position{ node.getPosition() };
			castor::Point3f direction{ nextPosition - position };
			auto distanceToDst = float( castor::point::length( direction ) );
			direction[0] *= distance / distanceToDst;
			direction[2] *= distance / distanceToDst;
			bool reachDst{ distanceToDst <= distance };
			bool result = false;

			if ( !reachDst )
			{
				// not at destination
				nextPosition = position + direction;
			}
			else if ( distanceToDst < distance )
			{
				// after destination => start walking to next destination
				++data.current;

				if ( data.current != data.path.end() )
				{
					data.destination = game.convert( castor::Point2i{ data.current->x, data.current->y } )
						+ castor::Point3f{ 0, game.getCellHeight() / 2, 0 };
					result = walkToDestination( game
						, data
						, speed
						, node
						, elapsed );
				}
				else
				{
					result = true;
					std::cout << "\nArrived at destination" << std::endl;
					node.setPosition( castor::Point3f{ 0, -10, 0 } );
				}
			}
			else
			{
				// Right on the destination node.
				++data.current;
				data.destination = game.convert( castor::Point2i{ data.current->x, data.current->y } )
					+ castor::Point3f{ 0, game.getCellHeight() / 2, 0 };
			}

			if ( !result )
			{
				node.setPosition( nextPosition );
			}

			return result;
		}

		State createWalkingState( Ecs & ecs
			, Entity const & entity )
		{
			auto walkData = ecs.getComponentData< WalkDataPtr >( entity
				, ecs.getComponent( Ecs::WalkComponent ) ).getValue();
			auto geometry = ecs.getComponentData< castor3d::GeometrySPtr >( entity
				, ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			auto node = Game::getEnemyNode( geometry );
			auto & speed = ecs.getComponentData< float >( entity
				, ecs.getComponent( Ecs::SpeedComponent ) );

			return State{ [&ecs, entity, walkData, node, &speed]( Game & game
				, Milliseconds const & elapsed
				, Milliseconds const & total )
			{
				auto life = ecs.getComponentData< uint32_t >( entity
					, ecs.getComponent( Ecs::LifeComponent ) ).getValue();

				if ( life )
				{
					auto const angle = castor::Angle::fromDegrees( -float( elapsed.count() ) * 240 / 1000.0f );
					node->yaw( angle );
					return walkToDestination( game
						, *walkData
						, speed.getValue()
						, *node
						, elapsed );
				}

				return true;
			} };
		}
	}
}
