#include "Tower.hpp"

#include "Ecs.hpp"

namespace orastus
{
	Tower::Tower( Ecs & p_ecs )
		: m_ecs{ p_ecs }
		, m_cooldown{ p_ecs.GetComponent( Ecs::CooldownComponent ) }
		, m_damage{ p_ecs.GetComponent( Ecs::DamageComponent ) }
		, m_range{ p_ecs.GetComponent( Ecs::RangeComponent ) }
		, m_bulletSpeed{ p_ecs.GetComponent( Ecs::SpeedComponent ) }
		, m_requiredLevel{ p_ecs.GetComponent( Ecs::LevelComponent ) }
		, m_state{ p_ecs.GetComponent( Ecs::StateComponent ) }
	{
	}

	void Tower::CreateData( Entity const & p_entity
		, Milliseconds const & p_cooldown
		, uint32_t p_damage
		, float p_range
		, float p_bulletSpeed
		, uint32_t p_requiredLevel )
	{
		m_ecs.CreateComponentData( p_entity, m_cooldown, p_cooldown );
		m_ecs.CreateComponentData( p_entity, m_damage, p_damage );
		m_ecs.CreateComponentData( p_entity, m_range, p_range );
		m_ecs.CreateComponentData( p_entity, m_bulletSpeed, p_bulletSpeed );
		m_ecs.CreateComponentData( p_entity, m_requiredLevel, p_requiredLevel );
		m_ecs.CreateComponentData( p_entity, m_state, TowerState::eIdle );
	}
}
