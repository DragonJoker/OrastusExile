#include "GameEngine/State/BulletState.hpp"

#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/Entity.hpp"
#include "GameEngine/ECS/TrackData.hpp"
#include "GameEngine/Game.hpp"

#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/SceneNode.hpp>

namespace orastus
{
	namespace bullet
	{
		State createTrackingState( Ecs & p_ecs, Entity const & p_entity )
		{
			auto l_trackData = p_ecs.getComponentData< TrackDataPtr >( p_entity
				, p_ecs.getComponent( Ecs::TrackComponent ) ).getValue();
			auto l_geometry = p_ecs.getComponentData< castor3d::GeometrySPtr >( p_entity
				, p_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			auto l_node = Game::getBulletNode( l_geometry );

			return State{ [&p_ecs, p_entity, l_trackData, l_node]( Game & p_game
				, Milliseconds const & p_elapsed
				, Milliseconds const & p_total )
			{
				auto l_target = Game::getEnemyNode( p_ecs.getComponentData< castor3d::GeometrySPtr >( l_trackData->target
					, p_ecs.getComponent( Ecs::GeometryComponent ) ).getValue() );
				auto l_distance = float( p_elapsed.count() ) * l_trackData->speed / 1000.0f;
				castor::Point3f l_nextPosition = l_target->getPosition();
				castor::Point3f l_position{ l_node->getPosition() };
				castor::Point3f l_direction{ l_nextPosition - l_position };
				auto l_distanceToDst = float( castor::point::length( l_direction ) );
				l_direction[0] *= l_distance / l_distanceToDst;
				l_direction[2] *= l_distance / l_distanceToDst;
				bool l_reachDst{ l_distanceToDst <= l_distance };
				bool l_result = false;

				if ( l_trackData->target )
				{
					auto l_life = p_ecs.getComponentData< uint32_t >( l_trackData->target
						, p_ecs.getComponent( Ecs::LifeComponent ) ).getValue();

					if ( l_life )
					{
						if ( !l_reachDst )
						{
							// not at destination
							l_nextPosition = l_position + l_direction;
							l_node->setPosition( l_nextPosition );
						}
						else if ( l_distanceToDst < l_distance )
						{
							p_game.hit( p_entity
								, l_trackData->target
								, l_trackData->damage );
							l_result = true;
						}
					}
					else
					{
						p_game.killBullet( p_entity );
						l_result = true;
					}
				}
				else
				{
					p_game.killBullet( p_entity );
					l_result = true;
				}

				return l_result;
			} };
		}
	}
}
