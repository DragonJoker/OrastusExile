#include "TowerState.hpp"

#include "ECS/AnimationData.hpp"
#include "ECS/AttackData.hpp"
#include "ECS/Ecs.hpp"
#include "ECS/Entity.hpp"
#include "Game.hpp"

#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/SceneNode.hpp>
#include <Castor3D/Scene/Animation/AnimatedObject.hpp>
#include <Castor3D/Scene/Animation/AnimatedObjectGroup.hpp>

namespace orastus
{
	String getName( TowerState p_state )
	{
		String l_result;

		switch ( p_state )
		{
		case TowerState::eEmpty:
			l_result = cuT( "Empty" );
			break;

		case TowerState::eIdle:
			l_result = cuT( "Idle" );
			break;

		case TowerState::eSpotted:
			l_result = cuT( "Spotted" );
			break;

		case TowerState::eShooting:
			l_result = cuT( "Shooting" );
			break;

		default:
			CU_Failure( "Unexpected tower state." );
			break;
		}

		return l_result;
	}

	namespace tower
	{
		bool doIsInRange( Ecs const & p_ecs
			, Entity const & p_enemy
			, castor3d::SceneNode const & p_towerNode
			, float p_range )
		{
			auto l_geometry = p_ecs.getComponentData< castor3d::GeometrySPtr >( p_enemy
				, p_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			auto l_enemyNode = Game::getEnemyNode( l_geometry );
			return castor::point::distance( p_towerNode.getPosition(), l_enemyNode->getPosition() ) <= p_range;
		}

		Entity doLookForEnemy( Ecs & p_ecs
			, EntityComponentsList const & p_enemies
			, castor3d::SceneNode const & p_towerNode
			, float p_range )
		{
			Entity l_result;

			for ( auto l_enemy = p_enemies.begin();
				l_enemy != p_enemies.end() && !l_result;
				++l_enemy )
			{
				if ( doIsInRange( p_ecs
						, l_enemy->entity
						, p_towerNode
						, p_range )
					&& componentCast< uint32_t >( *l_enemy->data ).getValue() )
				{
					l_result = l_enemy->entity;
				}
			}

			return l_result;
		}

		void doTurnToTarget( Ecs const & p_ecs
			, Entity const & p_enemy
			, castor3d::SceneNode & p_towerNode )
		{
			auto l_geometry = p_ecs.getComponentData< castor3d::GeometrySPtr >( p_enemy
				, p_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			auto l_enemyNode = Game::getEnemyNode( l_geometry );
			auto l_targetPosition = l_enemyNode->getDerivedPosition();
			l_targetPosition[1] = p_towerNode.getDerivedPosition()[1];
			auto l_direction = l_targetPosition - p_towerNode.getDerivedPosition();
			l_direction = castor::point::getNormalised( l_direction );
			castor::Point3f l_up{ 0, 1, 0 };
			auto l_right = castor::point::cross( l_direction, l_up );
			auto l_transform = castor::matrix::lookAt( p_towerNode.getDerivedPosition(), p_towerNode.getDerivedPosition() - l_direction, l_up );
			p_towerNode.setOrientation( castor::Quaternion::fromMatrix( l_transform ) );
		}

		State createIdleState( Ecs & p_ecs, Entity const & p_entity )
		{
			auto l_geometry = p_ecs.getComponentData< castor3d::GeometrySPtr >( p_entity
				, p_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();

			if ( !l_geometry )
			{
				throw std::runtime_error{ "No geometry component" };
			}

			auto l_node = Game::getTowerNode( l_geometry );

			if ( !l_node )
			{
				throw std::runtime_error{ "Geometry is not attached" };
			}

			auto l_range = p_ecs.getComponentData< float >( p_entity
				, p_ecs.getComponent( Ecs::RangeComponent ) ).getValue();

			return State{ [&p_ecs, p_entity, l_node, l_range]( Game & p_game
				, Milliseconds const & p_elapsed
				, Milliseconds const & p_total )
				{
					auto & l_enemies = p_ecs.getComponentDatas( p_ecs.getComponent( Ecs::LifeComponent ) );
					auto l_target = doLookForEnemy( p_ecs
						, l_enemies
						, *l_node
						, l_range );

					if ( l_target )
					{
						p_ecs.getComponentData< AttackDataPtr >( p_entity
							, p_ecs.getComponent( Ecs::AttackComponent ) ).getValue()->target = l_target;
					}

					return bool( l_target );
			} };
		}

		State createShootingState( Ecs & p_ecs, Entity const & p_entity )
		{
			auto l_animdata = p_ecs.getComponentData< AnimationDataPtr >( p_entity
				, p_ecs.getComponent( Ecs::AnimationComponent ) ).getValue();

			if ( !l_animdata )
			{
				throw std::runtime_error{ "No animation component" };
			}

			auto l_attackdata = p_ecs.getComponentData< AttackDataPtr >( p_entity
				, p_ecs.getComponent( Ecs::AttackComponent ) ).getValue();

			if ( !l_animdata )
			{
				throw std::runtime_error{ "No attack component" };
			}

			auto l_geometry = p_ecs.getComponentData< castor3d::GeometrySPtr >( p_entity
				, p_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();

			if ( !l_animdata )
			{
				throw std::runtime_error{ "No geometry component" };
			}

			auto l_node = Game::getTowerNode( l_geometry );

			if ( !l_node )
			{
				throw std::runtime_error{ "Geometry is not attached" };
			}

			return State{ [&p_ecs, p_entity, l_attackdata, l_animdata, l_node]( Game & p_game
				, Milliseconds const & p_elapsed
				, Milliseconds const & p_total )
			{
				auto l_result = false;

				if ( l_attackdata->target )
				{
					auto l_life = p_ecs.getComponentData< uint32_t >( l_attackdata->target
						, p_ecs.getComponent( Ecs::LifeComponent ) ).getValue();

					if ( !l_life )
					{
						auto l_range = p_ecs.getComponentData< float >( p_entity
							, p_ecs.getComponent( Ecs::RangeComponent ) ).getValue();
						l_attackdata->target = doLookForEnemy( p_ecs
							, p_ecs.getComponentDatas( p_ecs.getComponent( Ecs::LifeComponent ) )
							, *l_node
							, l_range );
					}
				}

				if ( l_attackdata->target )
				{
					doTurnToTarget( p_ecs
						, l_attackdata->target
						, *l_node );

					if ( !l_animdata->started )
					{
						l_animdata->animation->startAnimation( l_animdata->animName );
						l_animdata->started = true;
						l_attackdata->shot = false;
					}
					else
					{
						if ( p_total >= l_attackdata->shootDelay
							&& !l_attackdata->shot )
						{
							p_game.createBullet( p_entity
								, l_attackdata->target );
							l_attackdata->shot = true;
						}
						else if ( p_total >= l_animdata->total )
						{
							l_animdata->started = false;
							l_animdata->animation->stopAnimation( l_animdata->animName );
							l_animdata->animation->startAnimation( l_animdata->animName );
							l_animdata->animation->pauseAnimation( l_animdata->animName );
							l_result = true;
						}
					}
				}
				else if ( p_total >= l_animdata->total )
				{
					l_animdata->started = false;
					l_animdata->animation->stopAnimation( l_animdata->animName );
					l_animdata->animation->startAnimation( l_animdata->animName );
					l_animdata->animation->pauseAnimation( l_animdata->animName );
					l_result = true;
				}

				return l_result;
			} };
		}
	}
}
