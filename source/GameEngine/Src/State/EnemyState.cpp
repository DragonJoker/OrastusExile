#include "EnemyState.hpp"

#include "ECS/Ecs.hpp"
#include "ECS/Entity.hpp"
#include "ECS/WalkData.hpp"
#include "Game.hpp"

#include <Scene/Geometry.hpp>
#include <Scene/SceneNode.hpp>

namespace orastus
{
	String GetName( EnemyState p_state )
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
			FAILURE( "Unexpected enemy state." );
			break;
		}

		return l_result;
	}

	namespace enemy
	{
		bool WalkToDestination( Game & p_game
			, WalkData & p_data
			, float p_speed
			, Castor3D::SceneNode & p_node
			, Milliseconds const & p_elapsed )
		{
			auto & l_game = p_game;
			auto l_distance = p_elapsed.count() * p_speed / 1000;
			Castor::Point3r l_nextPosition = p_data.m_destination;
			Castor::Point3r l_position{ p_node.GetPosition() };
			Castor::Point3r l_direction{ l_nextPosition - l_position };
			auto l_distanceToDst = float( Castor::point::length( l_direction ) );
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
				++p_data.m_current;

				if ( p_data.m_current != p_data.m_path.end() )
				{
					p_data.m_destination = l_game.Convert( Castor::Point2i{ p_data.m_current->m_x, p_data.m_current->m_y } )
						+ Castor::Point3r{ 0, l_game.GetCellHeight(), 0 };
					l_result = WalkToDestination( p_game
						, p_data
						, p_speed
						, p_node
						, p_elapsed );
				}
				else
				{
					l_result = true;
					std::cout << "\nArrived at destination" << std::endl;
					p_node.SetPosition( Castor::Point3r{ 0, -10, 0 } );
				}
			}
			else
			{
				// Right on the destination node.
				++p_data.m_current;
				p_data.m_destination = l_game.Convert( Castor::Point2i{ p_data.m_current->m_x, p_data.m_current->m_y } )
					+ Castor::Point3r{ 0, l_game.GetCellHeight() / 2, 0 };
			}

			if ( !l_result )
			{
				p_node.SetPosition( l_nextPosition );
			}

			return l_result;
		}

		State CreateWalkingState( Ecs & p_ecs
			, Entity const & p_entity )
		{
			auto l_walkData = p_ecs.GetComponentData< WalkDataPtr >( p_entity
				, p_ecs.GetComponent( Ecs::WalkComponent ) ).GetValue();
			auto l_geometry = p_ecs.GetComponentData< Castor3D::GeometrySPtr >( p_entity
				, p_ecs.GetComponent( Ecs::GeometryComponent ) ).GetValue();
			auto l_node = Game::GetEnemyNode( l_geometry );
			auto & l_speed = p_ecs.GetComponentData< float >( p_entity
				, p_ecs.GetComponent( Ecs::SpeedComponent ) );

			return State{ [&p_ecs, p_entity, l_walkData, l_node, &l_speed]( Game & p_game
				, Milliseconds const & p_elapsed
				, Milliseconds const & p_total )
			{
				auto l_life = p_ecs.GetComponentData< uint32_t >( p_entity
					, p_ecs.GetComponent( Ecs::LifeComponent ) ).GetValue();

				if ( l_life )
				{
					auto const l_angle = Castor::Angle::from_degrees( -p_elapsed.count() * 120 / 1000.0_r );
					l_node->Yaw( l_angle );
					return WalkToDestination( p_game
						, *l_walkData
						, l_speed.GetValue()
						, *l_node
						, p_elapsed );
				}

				return true;
			} };
		}
	}
}
