#include "Bullet.hpp"

#include "Ecs.hpp"

namespace orastus
{
	Bullet::Bullet( Ecs & p_ecs )
		: m_ecs{ p_ecs }
		, m_speed{ m_ecs.GetComponent( Ecs::SpeedComponent ) }
		, m_damage{ m_ecs.GetComponent( Ecs::DamageComponent ) }
	{
	}

	void Bullet::CreateData( Entity const & p_entity
		, float p_speed
		, uint32_t p_damage )
	{
		m_ecs.CreateComponentData( p_entity, m_speed, p_speed );
		m_ecs.CreateComponentData( p_entity, m_damage, p_damage );
	}
}
