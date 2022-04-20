/**
See licence file in root folder, MIT.txt
*/
#include "GameEngine/System/TowerSystem.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/Enemy.hpp"
#include "GameEngine/ECS/Tower.hpp"

namespace orastus
{
	namespace tower
	{
		bool isInRange( Ecs const & ecs
			, EnemyData const & enemy
			, castor3d::SceneNode const & towerNode
			, float range )
		{
			auto enemyNode = Game::getEnemyNode( enemy.geometry );
			return castor::point::distance( towerNode.getPosition(), enemyNode->getPosition() ) <= range;
		}

		Entity lookForEnemy( Ecs const & ecs
			, EntityComponentsList const & enemies
			, castor3d::SceneNode const & towerNode
			, float range )
		{
			Entity result;

			for ( auto enemy = enemies.begin();
				enemy != enemies.end() && !result;
				++enemy )
			{
				auto & data = componentCast< EnemyData >( *enemy->data ).getValue();

				if ( isTargetable( data )
					&& isInRange( ecs, data, towerNode, range ) )
				{
					result = enemy->entity;
				}
			}

			return result;
		}

		void turnToTarget( Ecs const & ecs
			, Entity const & enemy
			, castor3d::SceneNode & towerNode )
		{
			auto enemyNode = Game::getEnemyNode( ecs.getComponentData< EnemyData >( enemy
				, ecs.getComponent( Ecs::EnemyStateComponent ) ).getValue().geometry );
			auto targetPosition = enemyNode->getDerivedPosition();
			targetPosition[1] = towerNode.getDerivedPosition()[1];
			auto direction = targetPosition - towerNode.getDerivedPosition();
			direction = castor::point::getNormalised( direction );
			castor::Point3f up{ 0, 1, 0 };
			auto right = castor::point::cross( direction, up );
			auto transform = castor::matrix::lookAt( towerNode.getDerivedPosition(), towerNode.getDerivedPosition() + direction, up );
			towerNode.setOrientation( castor::Quaternion::fromMatrix( transform ) );
		}

		bool updateTimeout( Milliseconds const & elapsed
			, TowerData & tower
			, Milliseconds const & defaultTimeout )
		{
			auto timeout = tower.timeout;

			if ( timeout < elapsed )
			{
				timeout = defaultTimeout;
			}
			else
			{
				timeout -= elapsed;
			}

			tower.timeout = timeout;
			return timeout == defaultTimeout;
		}

		void idle( Ecs const & ecs
			, Milliseconds const & elapsed
			, TowerData & tower )
		{
			auto node = Game::getTowerNode( tower.geometry );
			auto & enemies = ecs.getComponentDatas( ecs.getComponent( Ecs::EnemyStateComponent ) );
			auto target = lookForEnemy( ecs
				, enemies
				, *node
				, tower.category->getRange() );
			tower.attack->shot = false;

			if ( !tower.anim )
			{
				updateTimeout( elapsed, tower, 0_ms );
			}

			if ( target )
			{
				tower.attack->target = target;
				tower.status = TowerStatus::eShooting;
			}
		}

		void shoot( Game & game
			, Ecs & ecs
			, Milliseconds const & elapsed
			, TowerData & tower )
		{
			auto node = Game::getTowerNode( tower.geometry );

			if ( tower.attack->target )
			{
				auto & enemy = ecs.getComponentData< EnemyData >( tower.attack->target
					, ecs.getComponent( Ecs::EnemyStateComponent ) ).getValue();

				if ( !enemy.life )
				{
					tower.attack->target = lookForEnemy( ecs
						, ecs.getComponentDatas( ecs.getComponent( Ecs::EnemyStateComponent ) )
						, *node
						, tower.category->getRange() );
				}
			}

			auto & animdata = tower.anim;

			if ( !animdata )
			{
				if ( tower.attack->target )
				{
					turnToTarget( ecs
						, tower.attack->target
						, *node );

					if ( updateTimeout( elapsed, tower, tower.category->getCooldown() ) )
					{
						game.createBullet( tower, tower.attack->target );
						tower.status = TowerStatus::eIdle;
					}
				}
				else
				{
					updateTimeout( elapsed, tower, 0_ms );
					tower.status = TowerStatus::eIdle;
				}
			}
			else if ( tower.attack->target )
			{
				turnToTarget( ecs
					, tower.attack->target
					, *node );

				if ( !animdata->started )
				{
					animdata->animation->startAnimation( animdata->animName );
					animdata->started = true;
					tower.attack->shot = false;
				}
				else
				{
					auto atend = updateTimeout( elapsed, tower, animdata->total );

					if ( tower.timeout <= animdata->total - tower.attack->shootDelay
						&& !tower.attack->shot )
					{
						game.createBullet( tower, tower.attack->target );
						tower.attack->shot = true;
						tower.status = TowerStatus::eIdle;
					}
					else if ( atend )
					{
						animdata->started = false;
						animdata->animation->stopAnimation( animdata->animName );
						animdata->animation->startAnimation( animdata->animName );
						animdata->animation->pauseAnimation( animdata->animName );
						tower.status = TowerStatus::eIdle;
					}
				}
			}
			else if ( updateTimeout( elapsed, tower, 0_ms ) )
			{
				animdata->started = false;
				animdata->animation->stopAnimation( animdata->animName );
				animdata->animation->startAnimation( animdata->animName );
				animdata->animation->pauseAnimation( animdata->animName );
				tower.status = TowerStatus::eIdle;
			}
		}
	}

	TowerSystem::TowerSystem( Ecs & ecs )
		: m_ecs{ ecs }
	{
	}

	void TowerSystem::update( Game & game
		, Milliseconds const & elapsed )
	{
		auto & states = m_ecs.getComponentDatas( m_ecs.getComponent( Ecs::TowerStateComponent ) );

		for ( auto & data : states )
		{
			if ( data.data )
			{
				auto & towerData = static_cast< ComponentData< TowerData > & >( *data.data ).getValue();

				switch ( towerData.status )
				{
				case TowerStatus::eIdle:
					tower::idle( m_ecs, elapsed, towerData );
					break;
				case TowerStatus::eShooting:
					tower::shoot( game, m_ecs, elapsed, towerData );
					break;
				default:
					break;
				}
			}
		}
	}
}
