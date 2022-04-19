#include "GameEngine/State/EnemyState.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/Entity.hpp"
#include "GameEngine/ECS/WalkData.hpp"
#include "GameEngine/State/TargetState.hpp"

#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/SceneNode.hpp>

namespace orastus
{
	String getName( EnemyState state )
	{
		switch ( state )
		{
		case EnemyState::eSpawning:
			return cuT( "Spawning" );
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
		case EnemyState::eCapturing:
			return cuT( "Capturing" );
		case EnemyState::eEscaping:
			return cuT( "Escaping" );
		case EnemyState::eEscaped:
			return cuT( "Escaped" );
		default:
			CU_Failure( "Unexpected enemy state." );
			return "Unknown";
		}
	}

	namespace enemy
	{
		namespace details
		{
			bool spawn( Ecs & ecs
				, Entity const & entity
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
				direction[1] *= distance / distanceToDst;
				bool result{ distanceToDst <= distance };

				if ( !result )
				{
					// not at destination
					nextPosition = position + direction;
				}
				else
				{
					ecs.getComponentData< EnemyState >( entity
						, ecs.getComponent( Ecs::StatusComponent ) ).setValue( EnemyState::eWalking );
				}

				node.setPosition( nextPosition );
				return result;
			}

			bool wait( Game & game
				, Ecs & ecs
				, Entity const & entity
				, WalkData & data )
			{
				auto result = game.selectTarget();

				if ( result )
				{
					auto cell = ecs.getComponentData< GridCell >( result
						, ecs.getComponent( Ecs::CellComponent ) ).getValue();
					data.destination = game.convert( castor::Point2i{ cell.x, cell.y } );
					ecs.getComponentData< EnemyState >( entity
						, ecs.getComponent( Ecs::StatusComponent ) ).setValue( EnemyState::eSelecting );
					ecs.getComponentData< Entity >( entity
						, ecs.getComponent( Ecs::EntityComponent ) ).setValue( result );
				}
				else
				{
					ecs.getComponentData< EnemyState >( entity
						, ecs.getComponent( Ecs::StatusComponent ) ).setValue( EnemyState::eWaiting );
				}

				return bool( result );
			}

			bool walkToDestination( Game & game
				, Ecs & ecs
				, Entity const & entity
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
							+ castor::Point3f{ 0, game.getCellHeight() / 2.0f, 0 };
						result = walkToDestination( game
							, ecs
							, entity
							, data
							, speed
							, node
							, elapsed );
					}
					else
					{
						result = wait( game, ecs, entity, data );
					}
				}
				else
				{
					// Right on the destination node.
					++data.current;
					data.destination = game.convert( castor::Point2i{ data.current->x, data.current->y } )
						+ castor::Point3f{ 0, game.getCellHeight() / 2.0f, 0 };
				}

				if ( !result )
				{
					node.setPosition( nextPosition );
				}

				return result;
			}

			bool select( Game & game
				, Ecs & ecs
				, Entity const & entity
				, WalkData & data
				, float speed
				, castor3d::SceneNode & node
				, Milliseconds const & elapsed )
			{
				auto distance = float( elapsed.count() ) * speed / 1000.0f;
				castor::Point3f nextPosition = data.destination
					+ castor::Point3f{ 0, game.getCellHeight() * 2.0f, 0 };
				castor::Point3f position{ node.getPosition() };
				castor::Point3f direction{ nextPosition - position };
				auto distanceToDst = float( castor::point::length( direction ) );
				direction[0] *= distance / distanceToDst;
				direction[1] *= distance / distanceToDst;
				direction[2] *= distance / distanceToDst;
				bool result{ distanceToDst <= distance };

				if ( !result )
				{
					// not at destination
					nextPosition = position + direction;
				}
				else
				{
					auto target = ecs.getComponentData< Entity >( entity
						, ecs.getComponent( Ecs::EntityComponent ) ).getValue();
					CU_Require( target );
					ecs.getComponentData< TargetState >( target
						, ecs.getComponent( Ecs::StatusComponent ) ).setValue( TargetState::eCapturing );
					ecs.getComponentData< EnemyState >( entity
						, ecs.getComponent( Ecs::StatusComponent ) ).setValue( EnemyState::eCapturing );
				}

				node.setPosition( nextPosition );
				return result;
			}

			bool capture( Game & game
				, Ecs & ecs
				, Entity const & entity
				, Milliseconds const & elapsed )
			{
				auto time = ecs.getComponentData< Milliseconds >( entity
					, ecs.getComponent( Ecs::CooldownComponent ) ).getValue();
				auto result = time >= 2000_ms;

				if ( !result )
				{
					ecs.getComponentData< Milliseconds >( entity
						, ecs.getComponent( Ecs::CooldownComponent ) ).setValue( time + elapsed );
				}
				else
				{
					ecs.getComponentData< EnemyState >( entity
						, ecs.getComponent( Ecs::StatusComponent ) ).setValue( EnemyState::eEscaping );
					game.enemyEscaping( entity );
				}

				return result;
			}

			bool escape( Game & game
				, Ecs & ecs
				, Entity const & entity
				, WalkData & data
				, float speed
				, castor3d::SceneNode & node
				, Milliseconds const & elapsed )
			{
				auto distance = float( elapsed.count() ) * speed / 1000.0f;
				castor::Point3f nextPosition = data.destination
					+ castor::Point3f{ 0, game.getCellHeight() * 5.0f, 0 };
				castor::Point3f position{ node.getPosition() };
				castor::Point3f direction{ nextPosition - position };
				auto distanceToDst = float( castor::point::length( direction ) );
				direction[1] *= distance / distanceToDst;
				bool result{ distanceToDst <= distance };

				if ( !result )
				{
					// not at destination
					node.setPosition( position + direction );
				}
				else
				{
					ecs.getComponentData< EnemyState >( entity
						, ecs.getComponent( Ecs::StatusComponent ) ).setValue( EnemyState::eEscaped );
					game.enemyEscaped( entity );
				}

				return result;
			}
		}

		State createSpawningState( Ecs & ecs
			, Entity const & entity )
		{
			auto & walkData = ecs.getComponentData< WalkDataPtr >( entity
				, ecs.getComponent( Ecs::WalkComponent ) ).getValue();
			auto geometry = ecs.getComponentData< castor3d::GeometrySPtr >( entity
				, ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			auto node = Game::getEnemyNode( geometry );
			auto & speed = ecs.getComponentData< float >( entity
				, ecs.getComponent( Ecs::SpeedComponent ) );

			return State{ [&ecs, entity, &walkData, node, &speed]( Game & game
				, Milliseconds const & elapsed
				, Milliseconds const & total )
				{
					auto const angle = castor::Angle::fromDegrees( -float( elapsed.count() ) * 240 / 1000.0f );
					node->yaw( angle );
					return details::spawn( ecs
						, entity
						, *walkData
						, speed.getValue()
						, *node
						, elapsed );
				} };
		}

		State createWalkingState( Ecs & ecs
			, Entity const & entity )
		{
			auto & walkData = ecs.getComponentData< WalkDataPtr >( entity
				, ecs.getComponent( Ecs::WalkComponent ) ).getValue();
			auto geometry = ecs.getComponentData< castor3d::GeometrySPtr >( entity
				, ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			auto node = Game::getEnemyNode( geometry );
			auto & speed = ecs.getComponentData< float >( entity
				, ecs.getComponent( Ecs::SpeedComponent ) );

			return State{ [&ecs, entity, &walkData, node, &speed]( Game & game
				, Milliseconds const & elapsed
				, Milliseconds const & total )
			{
				bool result = true;
				auto life = ecs.getComponentData< uint32_t >( entity
					, ecs.getComponent( Ecs::LifeComponent ) ).getValue();

				if ( life )
				{
					auto const angle = castor::Angle::fromDegrees( -float( elapsed.count() ) * 240 / 1000.0f );
					node->yaw( angle );
					result = details::walkToDestination( game
						, ecs
						, entity
						, *walkData
						, speed.getValue()
						, *node
						, elapsed );
				}

				return result;
			} };
		}

		State createSelectingState( Ecs & ecs
			, Entity const & entity )
		{
			auto & walkData = ecs.getComponentData< WalkDataPtr >( entity
				, ecs.getComponent( Ecs::WalkComponent ) ).getValue();
			auto geometry = ecs.getComponentData< castor3d::GeometrySPtr >( entity
				, ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			auto node = Game::getEnemyNode( geometry );
			auto & speed = ecs.getComponentData< float >( entity
				, ecs.getComponent( Ecs::SpeedComponent ) );

			return State{ [&ecs, entity, &walkData, node, &speed]( Game & game
				, Milliseconds const & elapsed
				, Milliseconds const & total )
				{
					auto const angle = castor::Angle::fromDegrees( -float( elapsed.count() ) * 240 / 1000.0f );
					node->yaw( angle );
					return details::select( game
						, ecs
						, entity
						, *walkData
						, speed.getValue()
						, *node
						, elapsed );
			} };
		}

		State createCapturingState( Ecs & ecs
			, Entity const & entity )
		{
			auto geometry = ecs.getComponentData< castor3d::GeometrySPtr >( entity
				, ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			auto node = Game::getEnemyNode( geometry );

			return State{ [&ecs, entity, node]( Game & game
				, Milliseconds const & elapsed
				, Milliseconds const & total )
				{
					auto const angle = castor::Angle::fromDegrees( -float( elapsed.count() ) * 60 / 1000.0f );
					node->yaw( angle );
					return details::capture( game
						, ecs
						, entity
						, elapsed );
			} };
		}

		State createEscapingState( Ecs & ecs
			, Entity const & entity )
		{
			auto & walkData = ecs.getComponentData< WalkDataPtr >( entity
				, ecs.getComponent( Ecs::WalkComponent ) ).getValue();
			auto geometry = ecs.getComponentData< castor3d::GeometrySPtr >( entity
				, ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			auto node = Game::getEnemyNode( geometry );
			auto & speed = ecs.getComponentData< float >( entity
				, ecs.getComponent( Ecs::SpeedComponent ) );

			return State{ [&ecs, entity, &walkData, node, &speed]( Game & game
				, Milliseconds const & elapsed
				, Milliseconds const & total )
				{
					auto const angle = castor::Angle::fromDegrees( -float( elapsed.count() ) * 240 / 1000.0f );
					node->yaw( angle );
					return details::escape( game
						, ecs
						, entity
						, *walkData
						, speed.getValue()
						, *node
						, elapsed );
				} };
		}
	}
}
