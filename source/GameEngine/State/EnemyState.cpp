#include "EnemyState.hpp"

#include "ECS/Ecs.hpp"
#include "ECS/Entity.hpp"
#include "ECS/WalkData.hpp"
#include "Game.hpp"

#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/SceneNode.hpp>

namespace orastus
{
	String getName( EnemyState p_state )
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
			CU_Failure( "Unexpected enemy state." );
			break;
		}

		return l_result;
	}

	namespace enemy
	{
		bool walkToDestination( Game & p_game
			, WalkData & p_data
			, float p_speed
			, castor3d::SceneNode & p_node
			, Milliseconds const & p_elapsed )
		{
			auto & l_game = p_game;
			auto l_distance = float( p_elapsed.count() ) * p_speed / 1000.0f;
			castor::Point3f l_nextPosition = p_data.destination;
			castor::Point3f l_position{ p_node.getPosition() };
			castor::Point3f l_direction{ l_nextPosition - l_position };
			auto l_distanceToDst = float( castor::point::length( l_direction ) );
			l_direction[0] *= l_distance / l_distanceToDst;
			l_direction[2] *= l_distance / l_distanceToDst;
			bool l_reachDst{ l_distanceToDst <= l_distance };
			bool l_result = false;

			if ( !l_reachDst )
			{
				// not at destination
				l_nextPosition = l_position + l_direction;
			}
			else if ( l_distanceToDst < l_distance )
			{
				// after destination => start walking to next destination
				++p_data.current;

				if ( p_data.current != p_data.path.end() )
				{
					p_data.destination = l_game.convert( castor::Point2i{ p_data.current->m_x, p_data.current->m_y } )
						+ castor::Point3f{ 0, l_game.getCellHeight(), 0 };
					l_result = walkToDestination( p_game
						, p_data
						, p_speed
						, p_node
						, p_elapsed );
				}
				else
				{
					l_result = true;
					std::cout << "\nArrived at destination" << std::endl;
					p_node.setPosition( castor::Point3f{ 0, -10, 0 } );
				}
			}
			else
			{
				// Right on the destination node.
				++p_data.current;
				p_data.destination = l_game.convert( castor::Point2i{ p_data.current->m_x, p_data.current->m_y } )
					+ castor::Point3f{ 0, l_game.getCellHeight() / 2, 0 };
			}

			if ( !l_result )
			{
				p_node.setPosition( l_nextPosition );
			}

			return l_result;
		}

		State createWalkingState( Ecs & p_ecs
			, Entity const & p_entity )
		{
			auto l_walkData = p_ecs.getComponentData< WalkDataPtr >( p_entity
				, p_ecs.getComponent( Ecs::WalkComponent ) ).getValue();
			auto l_geometry = p_ecs.getComponentData< castor3d::GeometrySPtr >( p_entity
				, p_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			auto l_node = Game::getEnemyNode( l_geometry );
			auto & l_speed = p_ecs.getComponentData< float >( p_entity
				, p_ecs.getComponent( Ecs::SpeedComponent ) );

			return State{ [&p_ecs, p_entity, l_walkData, l_node, &l_speed]( Game & p_game
				, Milliseconds const & p_elapsed
				, Milliseconds const & p_total )
			{
				auto l_life = p_ecs.getComponentData< uint32_t >( p_entity
					, p_ecs.getComponent( Ecs::LifeComponent ) ).getValue();

				if ( l_life )
				{
					auto const l_angle = castor::Angle::fromDegrees( -float( p_elapsed.count() ) * 120 / 1000.0f );
					l_node->yaw( l_angle );
					return walkToDestination( p_game
						, *l_walkData
						, l_speed.getValue()
						, *l_node
						, p_elapsed );
				}

				return true;
			} };
		}
	}
}
