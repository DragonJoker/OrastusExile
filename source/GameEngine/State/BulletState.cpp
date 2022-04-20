#include "GameEngine/State/BulletState.hpp"

#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/Enemy.hpp"
#include "GameEngine/ECS/Entity.hpp"
#include "GameEngine/ECS/TrackData.hpp"
#include "GameEngine/Game.hpp"

#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/SceneNode.hpp>

namespace orastus
{
	namespace bullet
	{
		State createTrackingState( Ecs & ecs, Entity const & entity )
		{
			auto & trackData = ecs.getComponentData< TrackDataPtr >( entity
				, ecs.getComponent( Ecs::TrackComponent ) ).getValue();
			auto geometry = ecs.getComponentData< castor3d::GeometrySPtr >( entity
				, ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			auto node = Game::getBulletNode( geometry );

			return State{ [&ecs, entity, &trackData, node]( Game & game
				, Milliseconds const & elapsed
				, Milliseconds const & total )
			{
				auto target = Game::getEnemyNode( ecs.getComponentData< castor3d::GeometrySPtr >( trackData->target
					, ecs.getComponent( Ecs::GeometryComponent ) ).getValue() );
				auto distance = float( elapsed.count() ) * trackData->speed / 1000.0f;
				castor::Point3f nextPosition = target->getPosition();
				castor::Point3f position{ node->getPosition() };
				castor::Point3f direction{ nextPosition - position };
				auto distanceToDst = float( castor::point::length( direction ) );
				direction[0] *= distance / distanceToDst;
				direction[2] *= distance / distanceToDst;
				bool reachDst{ distanceToDst <= distance };
				bool result = false;

				if ( trackData->target )
				{
					if ( isTargetable( ecs
						, trackData->target
						, ecs.getComponentData< uint32_t >( trackData->target
							, ecs.getComponent( Ecs::LifeComponent ) ) ) )
					{
						if ( !reachDst )
						{
							// not at destination
							nextPosition = position + direction;
							node->setPosition( nextPosition );
						}
						else if ( distanceToDst < distance )
						{
							game.hit( entity
								, trackData->target
								, trackData->damage );
							result = true;
						}
					}
					else
					{
						game.killBullet( entity );
						result = true;
					}
				}
				else
				{
					game.killBullet( entity );
					result = true;
				}

				return result;
			} };
		}
	}
}
