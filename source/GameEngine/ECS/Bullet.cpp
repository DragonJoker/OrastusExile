#include "Bullet.hpp"

#include "ECS/Ecs.hpp"
#include "ECS/TrackData.hpp"
#include "State/BulletState.hpp"
#include "State/StateMachine.hpp"

#include <Castor3D/Scene/Geometry.hpp>

namespace orastus
{
	Bullet::Bullet( Ecs & p_ecs )
		: m_ecs{ p_ecs }
		, m_geometry{ m_ecs.getComponent( Ecs::GeometryComponent ) }
		, m_state{ m_ecs.getComponent( Ecs::StateComponent ) }
		, m_track{ m_ecs.getComponent( Ecs::TrackComponent ) }
	{
	}

	void Bullet::createData( Entity const & p_entity
		, castor3d::GeometrySPtr p_geometry
		, TrackDataPtr p_track )
	{
		m_ecs.createComponentData( p_entity
			, m_track
			, p_track );

		if ( p_geometry )
		{
			m_ecs.createComponentData( p_entity
				, m_geometry
				, p_geometry );
			m_ecs.createComponentData( p_entity
				, m_state
				, std::make_shared< StateMachine >( bullet::createTrackingState( m_ecs, p_entity ), false ) );
		}
	}

	void Bullet::resetData( Entity const & p_entity
		, castor3d::GeometrySPtr p_geometry
		, TrackDataPtr p_track )
	{
		m_ecs.getComponentData< castor3d::GeometrySPtr >( p_entity
			, m_geometry ).setValue( p_geometry );
		*m_ecs.getComponentData< TrackDataPtr >( p_entity
			, m_track ).getValue() = *p_track;
		m_ecs.getComponentData< StateMachinePtr >( p_entity
			, m_state ).getValue()->restart();
	}

	String Bullet::toString( Entity const & p_entity )
	{
		StringStream l_stream;
		l_stream << cuT( "Bullet(" ) << p_entity.getId() << cuT( ")" );
		l_stream << cuT( "\n Geometry: " ) << m_ecs.getComponentData< castor3d::GeometrySPtr >( p_entity, m_geometry ).getValue()->getName();
		l_stream << cuT( "\n Track: " ) << m_ecs.getComponentData< TrackDataPtr >( p_entity, m_track ).getValue()->target.getId();
		return l_stream.str();
	}
}
