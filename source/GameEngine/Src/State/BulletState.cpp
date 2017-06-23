#include "BulletState.hpp"

#include "ECS/Ecs.hpp"
#include "ECS/Entity.hpp"
#include "ECS/TrackData.hpp"
#include "Game.hpp"

#include <Scene/Geometry.hpp>
#include <Scene/SceneNode.hpp>

namespace orastus
{
	namespace bullet
	{
		State CreateTrackingState( Ecs & p_ecs, Entity const & p_entity )
		{
			auto l_trackData = p_ecs.GetComponentData< TrackDataPtr >( p_entity
				, p_ecs.GetComponent( Ecs::TrackComponent ) ).GetValue();
			auto l_geometry = p_ecs.GetComponentData< Castor3D::GeometrySPtr >( p_entity
				, p_ecs.GetComponent( Ecs::GeometryComponent ) ).GetValue();

			if ( l_trackData )
			{
				auto l_node = Game::GetBulletNode( l_geometry );

				return State{ [&p_ecs, p_entity, l_trackData, l_node]( Game & p_game
					, Milliseconds const & p_elapsed
					, Milliseconds const & p_total )
				{
					auto l_target = Game::GetEnemyNode( p_ecs.GetComponentData< Castor3D::GeometrySPtr >( l_trackData->m_target
						, p_ecs.GetComponent( Ecs::GeometryComponent ) ).GetValue() );
					auto l_distance = p_elapsed.count() * l_trackData->m_speed / 1000;
					Castor::Point3r l_nextPosition = l_target->GetPosition();
					Castor::Point3r l_position{ l_node->GetPosition() };
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
						l_node->SetPosition( l_nextPosition );
					}
					else if ( l_distanceToDst < l_distance )
					{
						p_game.Hit( p_entity
							, l_trackData->m_target
							, l_trackData->m_damage );
						l_result = true;
					}

					return l_result;
				} };
			}

			return State{ []( Game & p_game
				, Milliseconds const & p_elapsed
				, Milliseconds const & p_total )
			{
				return false;
			} };
		}
	}
}
