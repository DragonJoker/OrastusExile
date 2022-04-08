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
	SplashTower::SplashTower( Ecs & ecs )
		: m_ecs{ ecs }
		, m_cooldown{ m_ecs.getComponent( Ecs::CooldownComponent ) }
		, m_damage{ m_ecs.getComponent( Ecs::DamageComponent ) }
		, m_range{ m_ecs.getComponent( Ecs::RangeComponent ) }
		, m_bulletSpeed{ m_ecs.getComponent( Ecs::SpeedComponent ) }
		, m_requiredLevel{ m_ecs.getComponent( Ecs::LevelComponent ) }
		, m_state{ m_ecs.getComponent( Ecs::StateComponent ) }
		, m_splashDamage{ m_ecs.getComponent( Ecs::SplashDamageComponent ) }
		, m_splashRange{ m_ecs.getComponent( Ecs::SplashRangeComponent ) }
		, m_geometry{ m_ecs.getComponent( Ecs::GeometryComponent ) }
		, m_pickable{ m_ecs.getComponent( Ecs::PickableComponent ) }
		, m_animation{ m_ecs.getComponent( Ecs::AnimationComponent ) }
		, m_attack{ m_ecs.getComponent( Ecs::AttackComponent ) }
	{
	}

	void SplashTower::createData( Entity const & entity
		, Milliseconds const & cooldown
		, uint32_t damage
		, float range
		, float bulletSpeed
		, uint32_t splashDamage
		, float splashRange
		, uint32_t requiredLevel
		, castor3d::GeometrySPtr geometry
		, AnimationDataPtr animation
		, AttackDataPtr attack )
	{
		m_ecs.createComponentData( entity
			, m_cooldown
			, cooldown );
		m_ecs.createComponentData( entity
			, m_damage
			, damage );
		m_ecs.createComponentData( entity
			, m_range
			, range );
		m_ecs.createComponentData( entity
			, m_bulletSpeed
			, bulletSpeed );
		m_ecs.createComponentData( entity
			, m_requiredLevel
			, requiredLevel );
		m_ecs.createComponentData( entity
			, m_splashDamage
			, splashDamage );
		m_ecs.createComponentData( entity
			, m_splashRange
			, splashRange );
		m_ecs.createComponentData( entity
			, m_pickable
			, true );

		if ( animation )
		{
			m_ecs.createComponentData( entity
				, m_animation
				, animation );
		}

		m_ecs.createComponentData( entity
			, m_attack
			, attack );

		if ( geometry )
		{
			m_ecs.createComponentData( entity
				, m_geometry
				, geometry );
			m_ecs.createComponentData( entity
				, m_state
				, std::make_shared< StateMachine >( tower::createIdleState( m_ecs, entity ), true ) );
		}
	}

	String SplashTower::toString( Entity const & entity )
	{
		auto stream = castor::makeStringStream();
		stream << cuT( "SplashTower(" ) << entity.getId() << cuT( ")" );
		stream << cuT( "\n  Cooldown: " ) << m_ecs.getComponentData< Milliseconds >( entity, m_cooldown ).getValue().count() << " ms";
		stream << cuT( "\n  Damage: " ) << m_ecs.getComponentData< uint32_t >( entity, m_damage ).getValue();
		stream << cuT( "\n  Range: " ) << m_ecs.getComponentData< float >( entity, m_range ).getValue();
		stream << cuT( "\n  BulletSpeed: " ) << m_ecs.getComponentData< float >( entity, m_bulletSpeed ).getValue();
		stream << cuT( "\n  Level: " ) << m_ecs.getComponentData< uint32_t >( entity, m_requiredLevel ).getValue();
		stream << cuT( "\n  SplashDamage: " ) << m_ecs.getComponentData< uint32_t >( entity, m_splashDamage ).getValue();
		stream << cuT( "\n  SplashRange: " ) << m_ecs.getComponentData< float >( entity, m_splashRange ).getValue();
		stream << cuT( "\n  Geometry: " ) << m_ecs.getComponentData< castor3d::GeometrySPtr >( entity, m_geometry ).getValue()->getName();
		stream << cuT( "\n  Pickable: " ) << m_ecs.getComponentData< bool >( entity, m_pickable ).getValue();
		stream << cuT( "\n  Attack: " ) << m_ecs.getComponentData< AttackDataPtr >( entity, m_attack ).getValue()->target.getId();

		if ( m_ecs.hasComponent( entity, m_animation ) )
		{
			stream << cuT( "\n  Animation: " ) << m_ecs.getComponentData< AnimationDataPtr >( entity, m_animation ).getValue()->animation->getName();
		}

		return stream.str();
	}
}
