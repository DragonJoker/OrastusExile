#include "Enemy.hpp"

#include "Ecs.hpp"

namespace orastus
{
	Enemy::Enemy( Ecs & p_ecs )
		: m_ecs{ p_ecs }
		, m_speed{ p_ecs.GetComponent( Ecs::SpeedComponent ) }
		, m_life{ p_ecs.GetComponent( Ecs::LifeComponent ) }
		, m_state{ p_ecs.GetComponent( Ecs::StateComponent ) }
	{
	}

	void Enemy::CreateData( Entity const & p_entity
		, float p_speed
		, uint32_t p_life )
	{
		m_ecs.CreateComponentData( p_entity, m_speed, p_speed );
		m_ecs.CreateComponentData( p_entity, m_life, p_life );
		m_ecs.CreateComponentData( p_entity, m_state, EnemyState::eWalking );
	}
}
