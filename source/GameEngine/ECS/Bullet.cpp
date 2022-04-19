#include "GameEngine/ECS/Bullet.hpp"

#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/SoundSource.hpp"
#include "GameEngine/ECS/TrackData.hpp"
#include "GameEngine/State/BulletState.hpp"
#include "GameEngine/State/StateMachine.hpp"

#include <Castor3D/Scene/Geometry.hpp>

namespace orastus
{
	Bullet::Bullet( Ecs & ecs )
		: m_ecs{ ecs }
		, m_geometry{ m_ecs.getComponent( Ecs::GeometryComponent ) }
		, m_state{ m_ecs.getComponent( Ecs::StateComponent ) }
		, m_track{ m_ecs.getComponent( Ecs::TrackComponent ) }
		, m_hitSound{ m_ecs.getComponent( Ecs::SoundSourceComponent ) }
	{
	}

	void Bullet::createData( Entity const & entity
		, castor3d::GeometrySPtr geometry
		, SoundSource soundSource
		, TrackDataPtr track )
	{
		m_ecs.createComponentData( entity
			, m_track
			, std::move( track ) );

		if ( geometry )
		{
			m_ecs.createComponentData( entity
				, m_geometry
				, geometry );
			m_ecs.createComponentData( entity
				, m_state
				, std::make_unique< StateMachine >( bullet::createTrackingState( m_ecs, entity ), false ) );
			m_ecs.createComponentData( entity
				, m_hitSound
				, std::move( soundSource ) );
		}
	}

	void Bullet::resetData( Entity const & entity
		, castor3d::GeometrySPtr geometry
		, SoundSource soundSource
		, TrackDataPtr track )
	{
		m_ecs.getComponentData< castor3d::GeometrySPtr >( entity
			, m_geometry ).setValue( geometry );
		m_ecs.getComponentData< SoundSource >( entity
			, m_hitSound ).setValue( std::move( soundSource ) );
		*m_ecs.getComponentData< TrackDataPtr >( entity
			, m_track ).getValue() = *track;
		m_ecs.getComponentData< StateMachinePtr >( entity
			, m_state ).getValue()->restart();
	}

	String Bullet::toString( Entity const & entity )
	{
		auto stream = castor::makeStringStream();
		stream << cuT( "Bullet(" ) << entity.getId() << cuT( ")" );
		stream << cuT( "\n Geometry: " ) << m_ecs.getComponentData< castor3d::GeometrySPtr >( entity, m_geometry ).getValue()->getName();
		stream << cuT( "\n Track: " ) << m_ecs.getComponentData< TrackDataPtr >( entity, m_track ).getValue()->target.getId();
		return stream.str();
	}
}
