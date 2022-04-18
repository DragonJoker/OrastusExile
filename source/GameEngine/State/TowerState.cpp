#include "GameEngine/State/TowerState.hpp"

#include "GameEngine/ECS/AnimationData.hpp"
#include "GameEngine/ECS/AttackData.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/Entity.hpp"
#include "GameEngine/State/EnemyState.hpp"
#include "GameEngine/Game.hpp"

#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/SceneNode.hpp>
#include <Castor3D/Scene/Animation/AnimatedObject.hpp>
#include <Castor3D/Scene/Animation/AnimatedObjectGroup.hpp>

namespace orastus
{
	String getName( TowerState state )
	{
		switch ( state )
		{
		case TowerState::eEmpty:
			return cuT( "Empty" );
		case TowerState::eIdle:
			return cuT( "Idle" );
		case TowerState::eSpotted:
			return cuT( "Spotted" );
		case TowerState::eShooting:
			return cuT( "Shooting" );
		default:
			CU_Failure( "Unexpected tower state." );
			return "Unknown";
		}
	}

	namespace tower
	{
		bool isInRange( Ecs const & ecs
			, Entity const & enemy
			, castor3d::SceneNode const & towerNode
			, float range )
		{
			auto geometry = ecs.getComponentData< castor3d::GeometrySPtr >( enemy
				, ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			auto enemyNode = Game::getEnemyNode( geometry );
			return castor::point::distance( towerNode.getPosition(), enemyNode->getPosition() ) <= range;
		}

		bool isTargetable( Ecs const & ecs
			, Entity const & enemy
			, BaseComponentData const & lifeComponent )
		{
			auto status = ecs.getComponentData< EnemyState >( enemy
				, ecs.getComponent( Ecs::StatusComponent ) ).getValue();
			return status != EnemyState::eSpawning
				&& status != EnemyState::eEscaping
				&& status != EnemyState::eEscaped
				&& status != EnemyState::eDead
				&& status != EnemyState::eDying
				&& componentCast< uint32_t >( lifeComponent ).getValue();
		}

		Entity lookForEnemy( Ecs & ecs
			, EntityComponentsList const & enemies
			, castor3d::SceneNode const & towerNode
			, float range )
		{
			Entity result;

			for ( auto enemy = enemies.begin();
				enemy != enemies.end() && !result;
				++enemy )
			{
				if ( isTargetable( ecs
						, enemy->entity
						, *enemy->data )
					&& isInRange( ecs
						, enemy->entity
						, towerNode
						, range ) )
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
			auto geometry = ecs.getComponentData< castor3d::GeometrySPtr >( enemy
				, ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			auto enemyNode = Game::getEnemyNode( geometry );
			auto targetPosition = enemyNode->getDerivedPosition();
			targetPosition[1] = towerNode.getDerivedPosition()[1];
			auto direction = targetPosition - towerNode.getDerivedPosition();
			direction = castor::point::getNormalised( direction );
			castor::Point3f up{ 0, 1, 0 };
			auto right = castor::point::cross( direction, up );
			auto transform = castor::matrix::lookAt( towerNode.getDerivedPosition(), towerNode.getDerivedPosition() + direction, up );
			towerNode.setOrientation( castor::Quaternion::fromMatrix( transform ) );
		}

		State createIdleState( Ecs & ecs, Entity const & entity )
		{
			auto geometry = ecs.getComponentData< castor3d::GeometrySPtr >( entity
				, ecs.getComponent( Ecs::GeometryComponent ) ).getValue();

			if ( !geometry )
			{
				throw std::runtime_error{ "No geometry component" };
			}

			auto node = Game::getTowerNode( geometry );

			if ( !node )
			{
				throw std::runtime_error{ "Geometry is not attached" };
			}

			auto range = ecs.getComponentData< float >( entity
				, ecs.getComponent( Ecs::RangeComponent ) ).getValue();

			return State{ [&ecs, entity, node, range]( Game & game
				, Milliseconds const & elapsed
				, Milliseconds const & total )
				{
					auto & enemies = ecs.getComponentDatas( ecs.getComponent( Ecs::LifeComponent ) );
					auto target = lookForEnemy( ecs
						, enemies
						, *node
						, range );

					if ( target )
					{
						ecs.getComponentData< AttackDataPtr >( entity
							, ecs.getComponent( Ecs::AttackComponent ) ).getValue()->target = target;
					}

					return bool( target );
			} };
		}

		State createShootingState( Ecs & ecs, Entity const & entity )
		{
			auto animdata = ecs.hasComponent( entity, ecs.getComponent( Ecs::AnimationComponent ) )
				? ecs.getComponentData< AnimationDataPtr >( entity
					, ecs.getComponent( Ecs::AnimationComponent ) ).getValue().get()
				: nullptr;

			if ( !animdata )
			{
				if ( !ecs.hasComponent( entity, ecs.getComponent( Ecs::CooldownComponent ) ) )
				{
					throw std::runtime_error{ "Need either a cooldown or an animation component" };
				}
			}

			auto & attackdata = ecs.getComponentData< AttackDataPtr >( entity
				, ecs.getComponent( Ecs::AttackComponent ) ).getValue();

			if ( !attackdata )
			{
				throw std::runtime_error{ "No attack component" };
			}

			auto geometry = ecs.getComponentData< castor3d::GeometrySPtr >( entity
				, ecs.getComponent( Ecs::GeometryComponent ) ).getValue();

			if ( !geometry )
			{
				throw std::runtime_error{ "No geometry component" };
			}

			auto node = Game::getTowerNode( geometry );

			if ( !node )
			{
				throw std::runtime_error{ "Geometry is not attached" };
			}

			return State{ [&ecs, entity, &attackdata, animdata, node]( Game & game
				, Milliseconds const & elapsed
				, Milliseconds const & total )
			{
				auto result = false;

				if ( attackdata->target )
				{
					auto life = ecs.getComponentData< uint32_t >( attackdata->target
						, ecs.getComponent( Ecs::LifeComponent ) ).getValue();

					if ( !life )
					{
						auto range = ecs.getComponentData< float >( entity
							, ecs.getComponent( Ecs::RangeComponent ) ).getValue();
						attackdata->target = lookForEnemy( ecs
							, ecs.getComponentDatas( ecs.getComponent( Ecs::LifeComponent ) )
							, *node
							, range );
					}
				}

				if ( !animdata )
				{
					if ( attackdata->target )
					{
						turnToTarget( ecs
							, attackdata->target
							, *node );

						if ( total >= ecs.getComponentData< Milliseconds >( entity
								, ecs.getComponent( Ecs::CooldownComponent ) ).getValue() )
						{
							if ( !attackdata->shot )
							{
								game.createBullet( entity
									, attackdata->target );
								attackdata->shot = true;
							}

							result = true;
						}
						else
						{
							attackdata->shot = false;
						}
					}
					else if ( total >= ecs.getComponentData< Milliseconds >( entity
						, ecs.getComponent( Ecs::CooldownComponent ) ).getValue() )
					{
						result = true;
					}
				}
				else if ( attackdata->target )
				{
					turnToTarget( ecs
						, attackdata->target
						, *node );

					if ( !animdata->started )
					{
						animdata->animation->startAnimation( animdata->animName );
						animdata->started = true;
						attackdata->shot = false;
					}
					else
					{
						if ( total >= attackdata->shootDelay
							&& !attackdata->shot )
						{
							game.createBullet( entity
								, attackdata->target );
							attackdata->shot = true;
						}
						else if ( total >= animdata->total )
						{
							animdata->started = false;
							animdata->animation->stopAnimation( animdata->animName );
							animdata->animation->startAnimation( animdata->animName );
							animdata->animation->pauseAnimation( animdata->animName );
							result = true;
						}
					}
				}
				else if ( total >= animdata->total )
				{
					animdata->started = false;
					animdata->animation->stopAnimation( animdata->animName );
					animdata->animation->startAnimation( animdata->animName );
					animdata->animation->pauseAnimation( animdata->animName );
					result = true;
				}

				return result;
			} };
		}
	}
}
