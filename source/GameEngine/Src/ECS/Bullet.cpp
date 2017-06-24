#include "Bullet.hpp"

#include "ECS/Ecs.hpp"
#include "ECS/TrackData.hpp"
#include "State/BulletState.hpp"
#include "State/StateMachine.hpp"

#include <Scene/Geometry.hpp>

namespace orastus
{
	Bullet::Bullet( Ecs & p_ecs )
		: m_ecs{ p_ecs }
		, m_geometry{ m_ecs.GetComponent( Ecs::GeometryComponent ) }
		, m_state{ m_ecs.GetComponent( Ecs::StateComponent ) }
		, m_track{ m_ecs.GetComponent( Ecs::TrackComponent ) }
	{
	}

	void Bullet::CreateData( Entity const & p_entity
		, Castor3D::GeometrySPtr p_geometry
		, TrackDataPtr p_track )
	{
		m_ecs.CreateComponentData( p_entity
			, m_geometry
			, p_geometry );
		m_ecs.CreateComponentData( p_entity
			, m_track
			, p_track );
		m_ecs.CreateComponentData( p_entity
			, m_state
			, std::make_shared< StateMachine >( bullet::CreateTrackingState( m_ecs, p_entity ), false ) );
	}

	void Bullet::ResetData( Entity const & p_entity
		, Castor3D::GeometrySPtr p_geometry
		, TrackDataPtr p_track )
	{
		m_ecs.GetComponentData< Castor3D::GeometrySPtr >( p_entity
			, m_geometry ).SetValue( p_geometry );
		*m_ecs.GetComponentData< TrackDataPtr >( p_entity
			, m_track ).GetValue() = *p_track;
		m_ecs.GetComponentData< StateMachinePtr >( p_entity
			, m_state ).GetValue()->Restart();
	}

	String Bullet::ToString( Entity const & p_entity )
	{
		StringStream l_stream;
		l_stream << cuT( "Bullet(" ) << p_entity.GetId() << cuT( ")" );
		l_stream << cuT( "\n Geometry: " ) << m_ecs.GetComponentData< Castor3D::GeometrySPtr >( p_entity, m_geometry ).GetValue()->GetName();
		l_stream << cuT( "\n Track: " ) << m_ecs.GetComponentData< TrackDataPtr >( p_entity, m_track ).GetValue()->m_target.GetId();
		return l_stream.str();
	}
}
