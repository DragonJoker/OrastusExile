#include "TowerState.hpp"

#include "ECS/AnimationData.hpp"
#include "ECS/Ecs.hpp"
#include "ECS/Entity.hpp"
#include "Game.hpp"

#include <Scene/Geometry.hpp>
#include <Scene/SceneNode.hpp>
#include <Scene/Animation/AnimatedObject.hpp>
#include <Scene/Animation/AnimatedObjectGroup.hpp>

namespace orastus
{
	String GetName( TowerState p_state )
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
			FAILURE( "Unexpected tower state." );
			break;
		}

		return l_result;
	}

	namespace tower
	{
		bool DoIsInRange( Ecs const & p_ecs
			, Entity const & p_enemy
			, Castor3D::SceneNode const & p_node
			, float p_range )
		{
			auto l_geometry = p_ecs.GetComponentData< Castor3D::GeometrySPtr >( p_enemy
				, p_ecs.GetComponent( Ecs::GeometryComponent ) ).GetValue();
			auto l_node = l_geometry->GetParent()->GetParent();
			return Castor::point::length( p_node.GetPosition() - l_node->GetPosition() ) <= p_range;
		}

		Entity DoLookForEnemy( Ecs & p_ecs
			, EntityComponentsList & p_enemies
			, Castor3D::SceneNode const & p_node
			, float p_range )
		{
			Entity l_result;

			for ( auto & l_it = p_enemies.begin(); l_it != p_enemies.end() && !l_result; ++l_it )
			{
				auto & l_enemy = *l_it;
				auto l_life = ComponentCast< uint32_t >( *l_enemy.m_data ).GetValue();

				if ( l_life > 0
					&& DoIsInRange( p_ecs, l_enemy.m_entity, p_node, p_range ) )
				{
					l_result = l_enemy.m_entity;
				}
			}

			return l_result;
		}

		State CreateIdleState( Ecs & p_ecs, Entity const & p_entity )
		{
			auto l_geometry = p_ecs.GetComponentData< Castor3D::GeometrySPtr >( p_entity
				, p_ecs.GetComponent( Ecs::GeometryComponent ) ).GetValue();

			if ( l_geometry )
			{
				auto l_node = l_geometry->GetParent();
				auto l_range = p_ecs.GetComponentData< float >( p_entity
					, p_ecs.GetComponent( Ecs::RangeComponent ) ).GetValue();

				return State{ [&p_ecs, p_entity, l_node, l_range]( Milliseconds const & p_elapsed
					, Milliseconds const & p_total )
				{
					auto & l_enemies = p_ecs.GetComponentDatas( p_ecs.GetComponent( Ecs::LifeComponent ) );
					auto l_target = DoLookForEnemy( p_ecs, l_enemies, *l_node, l_range );

					if ( l_target )
					{
						p_ecs.GetComponentData< TowerAttackDataPtr >( p_entity
							, p_ecs.GetComponent( Ecs::AttackComponent ) ).GetValue()->m_target = l_target;
					}

					return bool( l_target );
				} };
			}

			return State{ []( Milliseconds const & p_elapsed
				, Milliseconds const & p_total )
			{
				return false;
			} };
		}

		State CreateShootingState( Ecs & p_ecs, Entity const & p_entity )
		{
			auto l_animdata = p_ecs.GetComponentData< AnimationDataPtr >( p_entity
				, p_ecs.GetComponent( Ecs::AnimationComponent ) ).GetValue();
			auto l_attackdata = p_ecs.GetComponentData< TowerAttackDataPtr >( p_entity
				, p_ecs.GetComponent( Ecs::AttackComponent ) ).GetValue();

			if ( l_animdata && l_attackdata )
			{
				return State{ [&p_ecs, p_entity, l_attackdata, l_animdata]( Milliseconds const & p_elapsed
					, Milliseconds const & p_total )
				{
					auto l_result = false;

					if ( !l_animdata->m_started )
					{
						l_animdata->m_animation->StartAnimation( l_animdata->m_animName );
						l_animdata->m_started = true;
						l_attackdata->m_shot = false;
					}
					else if ( p_total >= l_attackdata->m_shootDelay
						&& !l_attackdata->m_shot )
					{
						l_animdata->m_game.CreateBullet( p_entity
							, l_attackdata->m_target );
						l_attackdata->m_shot = true;
					}
					else if ( p_total >= l_animdata->m_total )
					{
						l_animdata->m_started = false;
						l_animdata->m_animation->StopAnimation( l_animdata->m_animName );
						l_animdata->m_animation->StartAnimation( l_animdata->m_animName );
						l_animdata->m_animation->PauseAnimation( l_animdata->m_animName );
						l_result = true;
					}

					return l_result;
				} };
			}

			return State{ []( Milliseconds const & p_elapsed
				, Milliseconds const & p_total )
			{
				return false;
			} };
		}
	}
}
