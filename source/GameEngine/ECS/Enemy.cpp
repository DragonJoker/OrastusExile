#include "GameEngine/ECS/Enemy.hpp"

#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/WalkData.hpp"
#include "GameEngine/State/EnemyState.hpp"
#include "GameEngine/State/StateMachine.hpp"

namespace orastus
{
	Enemy::Enemy( Ecs & p_ecs )
		: m_ecs{ p_ecs }
		, m_speed{ p_ecs.getComponent( Ecs::SpeedComponent ) }
		, m_life{ p_ecs.getComponent( Ecs::LifeComponent ) }
		, m_geometry{ p_ecs.getComponent( Ecs::GeometryComponent ) }
		, m_walk{ p_ecs.getComponent( Ecs::WalkComponent ) }
		, m_state{ p_ecs.getComponent( Ecs::StateComponent ) }
	{
	}

	void Enemy::createData( Entity const & p_entity
		, float p_speed
		, uint32_t p_life
		, castor3d::GeometrySPtr p_geometry
		, WalkDataPtr p_walkData )
	{
		m_ecs.createComponentData( p_entity
			, m_speed
			, p_speed );
		m_ecs.createComponentData( p_entity
			, m_life
			, p_life );
		m_ecs.createComponentData( p_entity
			, m_walk
			, p_walkData );

		if ( p_geometry )
		{
			m_ecs.createComponentData( p_entity
				, m_geometry
				, p_geometry );
			m_ecs.createComponentData( p_entity
				, m_state
				, std::make_shared< StateMachine >( enemy::createWalkingState( m_ecs, p_entity ), false ) );
		}
	}

	void Enemy::resetData( Entity const & p_entity
		, float p_speed
		, uint32_t p_life
		, castor3d::GeometrySPtr p_geometry
		, WalkDataPtr p_walkData )
	{
		m_ecs.getComponentData< float >( p_entity
			, m_speed ).setValue( p_speed );
		m_ecs.getComponentData< uint32_t >( p_entity
			, m_life ).setValue( p_life );
		*m_ecs.getComponentData< WalkDataPtr >( p_entity
			, m_walk ).getValue() = *p_walkData;
		m_ecs.getComponentData< StateMachinePtr >( p_entity
			, m_state ).getValue()->restart();
	}

	String Enemy::toString( Entity const & p_entity )
	{
		StringStream l_stream;
		l_stream << cuT( "Enemy(" ) << p_entity.getId() << cuT( ")" );
		l_stream << cuT( "\n Speed: " ) << m_ecs.getComponentData< float >( p_entity, m_speed ).getValue();
		l_stream << cuT( "\n Life: " ) << m_ecs.getComponentData< uint32_t >( p_entity, m_life ).getValue();
		l_stream << cuT( "\n  Geometry: " ) << m_ecs.getComponentData< castor3d::GeometrySPtr >( p_entity, m_geometry ).getValue()->getName();
		//l_stream << cuT( "\n State: " ) << getName( m_ecs.getComponentData< EnemyState >( p_entity, m_state ).getValue() );
		return l_stream.str();
	}
}
