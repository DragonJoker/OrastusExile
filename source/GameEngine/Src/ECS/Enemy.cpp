#include "Enemy.hpp"

#include "ECS/Ecs.hpp"
#include "State/EnemyState.hpp"
#include "State/StateMachine.hpp"

namespace orastus
{
	Enemy::Enemy( Ecs & p_ecs )
		: m_ecs{ p_ecs }
		, m_speed{ p_ecs.GetComponent( Ecs::SpeedComponent ) }
		, m_life{ p_ecs.GetComponent( Ecs::LifeComponent ) }
		, m_geometry{ p_ecs.GetComponent( Ecs::GeometryComponent ) }
		, m_walk{ p_ecs.GetComponent( Ecs::WalkComponent ) }
		, m_state{ p_ecs.GetComponent( Ecs::StateComponent ) }
	{
	}

	void Enemy::CreateData( Entity const & p_entity
		, float p_speed
		, uint32_t p_life
		, Castor3D::GeometrySPtr p_geometry
		, WalkDataPtr p_walkData )
	{
		m_ecs.CreateComponentData( p_entity
			, m_speed
			, p_speed );
		m_ecs.CreateComponentData( p_entity
			, m_life
			, p_life );
		m_ecs.CreateComponentData( p_entity
			, m_geometry
			, p_geometry );
		m_ecs.CreateComponentData( p_entity
			, m_walk
			, p_walkData );
		m_ecs.CreateComponentData( p_entity
			, m_state
			, std::make_shared< StateMachine >( enemy::CreateWalkingState( m_ecs, p_entity ), false ) );
	}

	void Enemy::ResetData( Entity const & p_entity
		, float p_speed
		, uint32_t p_life
		, Castor3D::GeometrySPtr p_geometry
		, WalkDataPtr p_walkData )
	{
		m_ecs.GetComponentData< float >( p_entity
			, m_speed ).SetValue( p_speed );
		m_ecs.GetComponentData< uint32_t >( p_entity
			, m_life ).SetValue( p_life );
		m_ecs.GetComponentData< Castor3D::GeometrySPtr >( p_entity
			, m_geometry ).SetValue( p_geometry );
		m_ecs.GetComponentData< WalkDataPtr >( p_entity
			, m_walk ).SetValue( p_walkData );
		m_ecs.GetComponentData< StateMachinePtr >( p_entity
			, m_state ).GetValue()->Restart();
	}

	String Enemy::ToString( Entity const & p_entity )
	{
		StringStream l_stream;
		l_stream << cuT( "Enemy(" ) << p_entity.GetId() << cuT( ")" );
		l_stream << cuT( "\n Speed: " ) << m_ecs.GetComponentData< float >( p_entity, m_speed ).GetValue();
		l_stream << cuT( "\n Life: " ) << m_ecs.GetComponentData< uint32_t >( p_entity, m_life ).GetValue();
		l_stream << cuT( "\n  Geometry: " ) << m_ecs.GetComponentData< Castor3D::GeometrySPtr >( p_entity, m_geometry ).GetValue()->GetName();
		//l_stream << cuT( "\n State: " ) << GetName( m_ecs.GetComponentData< EnemyState >( p_entity, m_state ).GetValue() );
		return l_stream.str();
	}
}
