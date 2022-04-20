#include "GameEngine/System/BulletSystem.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/ECS/Bullet.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/Enemy.hpp"
#include "GameEngine/ECS/Entity.hpp"

#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/SceneNode.hpp>

namespace orastus
{
	String getName( BulletStatus state )
	{
		switch ( state )
		{
		case BulletStatus::eIdle:
			return cuT( "Idle" );
		case BulletStatus::eFlying:
			return cuT( "Flying" );
		default:
			CU_Failure( "Unexpected bullet state." );
			return "Unknown";
		}
	}

	namespace bullet
	{
		void fly( Game & game
			, Ecs & ecs
			, Milliseconds const & elapsed
			, BulletData & bullet )
		{
			auto & trackData = bullet.track;
			auto node = Game::getBulletNode( bullet.geometry );
			auto target = Game::getEnemyNode( ecs.getComponentData< EnemyData >( trackData->target
				, ecs.getComponent( Ecs::EnemyStateComponent ) ).getValue().geometry );
			auto distance = float( elapsed.count() ) * trackData->speed / 1000.0f;
			castor::Point3f nextPosition = target->getPosition();
			castor::Point3f position{ node->getPosition() };
			castor::Point3f direction{ nextPosition - position };
			auto distanceToDst = float( castor::point::length( direction ) );
			direction[0] *= distance / distanceToDst;
			direction[2] *= distance / distanceToDst;
			bool reachDst{ distanceToDst <= distance };

			if ( trackData->target )
			{
				if ( isTargetable( ecs, trackData->target ) )
				{
					if ( !reachDst )
					{
						// not at destination
						nextPosition = position + direction;
						node->setPosition( nextPosition );
					}
					else if ( distanceToDst < distance )
					{
						game.hit( bullet
							, trackData->target
							, trackData->damage );
						bullet.status = BulletStatus::eIdle;
					}
				}
				else
				{
					game.killBullet( bullet );
					bullet.status = BulletStatus::eIdle;
				}
			}
			else
			{
				game.killBullet( bullet );
				bullet.status = BulletStatus::eIdle;
			}
		}
	}

	BulletSystem::BulletSystem( Ecs & ecs )
		: m_ecs{ ecs }
	{
	}

	void BulletSystem::update( Game & game
		, Milliseconds const & elapsed )
	{
		auto & states = m_ecs.getComponentDatas( m_ecs.getComponent( Ecs::BulletStateComponent ) );

		for ( auto & data : states )
		{
			if ( data.data )
			{
				auto & bullet = static_cast< ComponentData< BulletData > & >( *data.data ).getValue();

				switch ( bullet.status )
				{
				case BulletStatus::eIdle:
					break;
				case BulletStatus::eFlying:
					bullet::fly( game, m_ecs, elapsed, bullet );
					break;
				default:
					break;
				}
			}
		}
	}
}
