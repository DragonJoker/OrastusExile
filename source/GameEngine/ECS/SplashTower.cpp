#include "GameEngine/ECS/SplashTower.hpp"

#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/AnimationData.hpp"
#include "GameEngine/ECS/AttackData.hpp"
#include "GameEngine/State/TowerState.hpp"
#include "GameEngine/State/StateMachine.hpp"

#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/Animation/AnimatedObjectGroup.hpp>

namespace orastus
{
	SplashTower::SplashTower( Ecs & p_ecs )
		: m_ecs{ p_ecs }
		, m_cooldown{ p_ecs.getComponent( Ecs::CooldownComponent ) }
		, m_damage{ p_ecs.getComponent( Ecs::DamageComponent ) }
		, m_range{ p_ecs.getComponent( Ecs::RangeComponent ) }
		, m_bulletSpeed{ p_ecs.getComponent( Ecs::SpeedComponent ) }
		, m_requiredLevel{ p_ecs.getComponent( Ecs::LevelComponent ) }
		, m_state{ p_ecs.getComponent( Ecs::StateComponent ) }
		, m_splashDamage{ p_ecs.getComponent( Ecs::SplashDamageComponent ) }
		, m_splashRange{ p_ecs.getComponent( Ecs::SplashRangeComponent ) }
		, m_geometry{ p_ecs.getComponent( Ecs::GeometryComponent ) }
		, m_pickable{ p_ecs.getComponent( Ecs::PickableComponent ) }
		, m_animation{ p_ecs.getComponent( Ecs::AnimationComponent ) }
		, m_attack{ p_ecs.getComponent( Ecs::AttackComponent ) }
	{
	}

	void SplashTower::createData( Entity const & p_entity
		, Milliseconds const & p_cooldown
		, uint32_t p_damage
		, float p_range
		, float p_bulletSpeed
		, uint32_t p_splashDamage
		, float p_splashRange
		, uint32_t p_requiredLevel
		, castor3d::GeometrySPtr p_geometry
		, AnimationDataPtr p_animation
		, AttackDataPtr p_attack )
	{
		m_ecs.createComponentData( p_entity
			, m_cooldown
			, p_cooldown );
		m_ecs.createComponentData( p_entity
			, m_damage
			, p_damage );
		m_ecs.createComponentData( p_entity
			, m_range
			, p_range );
		m_ecs.createComponentData( p_entity
			, m_bulletSpeed
			, p_bulletSpeed );
		m_ecs.createComponentData( p_entity
			, m_requiredLevel
			, p_requiredLevel );
		m_ecs.createComponentData( p_entity
			, m_splashDamage
			, p_splashDamage );
		m_ecs.createComponentData( p_entity
			, m_splashRange
			, p_splashRange );
		m_ecs.createComponentData( p_entity
			, m_pickable
			, true );
		m_ecs.createComponentData( p_entity
			, m_animation
			, p_animation );
		m_ecs.createComponentData( p_entity
			, m_attack
			, p_attack );

		if ( p_geometry )
		{
			m_ecs.createComponentData( p_entity
				, m_geometry
				, p_geometry );
			m_ecs.createComponentData( p_entity
				, m_state
				, std::make_shared< StateMachine >( tower::createIdleState( m_ecs, p_entity ), true ) );
		}
	}

	String SplashTower::toString( Entity const & p_entity )
	{
		StringStream l_stream;
		l_stream << cuT( "SplashTower(" ) << p_entity.getId() << cuT( ")" );
		l_stream << cuT( "\n  Cooldown: " ) << m_ecs.getComponentData< Milliseconds >( p_entity, m_cooldown ).getValue().count() << " ms";
		l_stream << cuT( "\n  Damage: " ) << m_ecs.getComponentData< uint32_t >( p_entity, m_damage ).getValue();
		l_stream << cuT( "\n  Range: " ) << m_ecs.getComponentData< float >( p_entity, m_range ).getValue();
		l_stream << cuT( "\n  BulletSpeed: " ) << m_ecs.getComponentData< float >( p_entity, m_bulletSpeed ).getValue();
		l_stream << cuT( "\n  Level: " ) << m_ecs.getComponentData< uint32_t >( p_entity, m_requiredLevel ).getValue();
		l_stream << cuT( "\n  SplashDamage: " ) << m_ecs.getComponentData< uint32_t >( p_entity, m_splashDamage ).getValue();
		l_stream << cuT( "\n  SplashRange: " ) << m_ecs.getComponentData< float >( p_entity, m_splashRange ).getValue();
		l_stream << cuT( "\n  Geometry: " ) << m_ecs.getComponentData< castor3d::GeometrySPtr >( p_entity, m_geometry ).getValue()->getName();
		l_stream << cuT( "\n  Pickable: " ) << m_ecs.getComponentData< bool >( p_entity, m_pickable ).getValue();
		l_stream << cuT( "\n  Animation: " ) << m_ecs.getComponentData< AnimationDataPtr >( p_entity, m_animation ).getValue()->animation->getName();
		l_stream << cuT( "\n  Attack: " ) << m_ecs.getComponentData< AttackDataPtr >( p_entity, m_attack ).getValue()->target.getId();
		return l_stream.str();
	}
}
