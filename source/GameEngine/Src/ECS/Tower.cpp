#include "Tower.hpp"

#include "ECS/Ecs.hpp"
#include "ECS/AnimationData.hpp"
#include "ECS/AttackData.hpp"
#include "State/TowerState.hpp"
#include "State/StateMachine.hpp"

#include <Scene/Geometry.hpp>
#include <Scene/Animation/AnimatedObjectGroup.hpp>

namespace orastus
{
	Tower::Tower( Ecs & p_ecs )
		: m_ecs{ p_ecs }
		, m_cooldown{ p_ecs.GetComponent( Ecs::CooldownComponent ) }
		, m_damage{ p_ecs.GetComponent( Ecs::DamageComponent ) }
		, m_range{ p_ecs.GetComponent( Ecs::RangeComponent ) }
		, m_bulletSpeed{ p_ecs.GetComponent( Ecs::SpeedComponent ) }
		, m_requiredLevel{ p_ecs.GetComponent( Ecs::LevelComponent ) }
		, m_geometry{ p_ecs.GetComponent( Ecs::GeometryComponent ) }
		, m_pickable{ p_ecs.GetComponent( Ecs::PickableComponent ) }
		, m_animation{ p_ecs.GetComponent( Ecs::AnimationComponent ) }
		, m_state{ p_ecs.GetComponent( Ecs::StateComponent ) }
		, m_attack{ p_ecs.GetComponent( Ecs::AttackComponent ) }
	{
	}

	void Tower::CreateData( Entity const & p_entity
		, Milliseconds const & p_cooldown
		, uint32_t p_damage
		, float p_range
		, float p_bulletSpeed
		, uint32_t p_requiredLevel
		, Castor3D::GeometrySPtr p_geometry
		, AnimationDataPtr p_animation
		, AttackDataPtr p_attack )
	{
		m_ecs.CreateComponentData( p_entity
			, m_cooldown
			, p_cooldown );
		m_ecs.CreateComponentData( p_entity
			, m_damage
			, p_damage );
		m_ecs.CreateComponentData( p_entity
			, m_range
			, p_range );
		m_ecs.CreateComponentData( p_entity
			, m_bulletSpeed
			, p_bulletSpeed );
		m_ecs.CreateComponentData( p_entity
			, m_requiredLevel
			, p_requiredLevel );
		m_ecs.CreateComponentData( p_entity
			, m_geometry
			, p_geometry );
		m_ecs.CreateComponentData( p_entity
			, m_pickable
			, true );
		m_ecs.CreateComponentData( p_entity
			, m_animation
			, p_animation );
		m_ecs.CreateComponentData( p_entity
			, m_attack
			, p_attack );
		m_ecs.CreateComponentData( p_entity
			, m_state
			, std::make_shared< StateMachine >( tower::CreateIdleState( m_ecs, p_entity ), true ) );
	}

	String Tower::ToString( Entity const & p_entity )
	{
		StringStream l_stream;
		l_stream << cuT( "Tower(" ) << p_entity.GetId() << cuT( ")" );
		l_stream << cuT( "\n  Cooldown: " ) << m_ecs.GetComponentData< Milliseconds >( p_entity, m_cooldown ).GetValue().count() << " ms";
		l_stream << cuT( "\n  Damage: " ) << m_ecs.GetComponentData< uint32_t >( p_entity, m_damage ).GetValue();
		l_stream << cuT( "\n  Range: " ) << m_ecs.GetComponentData< float >( p_entity, m_range ).GetValue();
		l_stream << cuT( "\n  BulletSpeed: " ) << m_ecs.GetComponentData< float >( p_entity, m_bulletSpeed ).GetValue();
		l_stream << cuT( "\n  Level: " ) << m_ecs.GetComponentData< uint32_t >( p_entity, m_requiredLevel ).GetValue();
		l_stream << cuT( "\n  Geometry: " ) << m_ecs.GetComponentData< Castor3D::GeometrySPtr >( p_entity, m_geometry ).GetValue()->GetName();
		l_stream << cuT( "\n  Pickable: " ) << m_ecs.GetComponentData< bool >( p_entity, m_pickable ).GetValue();
		l_stream << cuT( "\n  Animation: " ) << m_ecs.GetComponentData< AnimationDataPtr >( p_entity, m_animation ).GetValue()->m_animation->GetName();
		l_stream << cuT( "\n  Attack: " ) << m_ecs.GetComponentData< AttackDataPtr >( p_entity, m_attack ).GetValue()->m_target.GetId();
		return l_stream.str();
	}
}
