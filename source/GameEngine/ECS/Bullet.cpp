#include "GameEngine/ECS/Bullet.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/ECS/Bullet.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/SoundSource.hpp"

#include <Castor3D/Scene/Geometry.hpp>

namespace orastus
{
	//*********************************************************************************************

	BulletData::BulletData( Entity pentity
		, castor3d::GeometrySPtr pgeometry
		, TrackDataPtr ptrack )
		: entity{ pentity }
		, status{ BulletStatus::eFlying }
		, geometry{ std::move( pgeometry ) }
		, track{ std::move( ptrack ) }
	{
		if ( !track )
		{
			throw std::runtime_error{ "No track data" };
		}

		if ( !geometry )
		{
			throw std::runtime_error{ "No geometry" };
		}

		auto node = Game::getBulletNode( geometry );

		if ( !node )
		{
			throw std::runtime_error{ "Geometry is not attached" };
		}
	}

	//*********************************************************************************************

	Bullet::Bullet( Ecs & ecs )
		: m_ecs{ ecs }
		, m_bullet{ m_ecs.getComponent( Ecs::BulletStateComponent ) }
		, m_hitSound{ m_ecs.getComponent( Ecs::SoundSourceComponent ) }
	{
	}

	void Bullet::createData( Entity const & entity
		, castor3d::GeometrySPtr geometry
		, SoundSource const * soundSource
		, TrackDataPtr track )
	{
		m_ecs.createComponentData( entity
			, m_bullet
			, BulletData{ entity
				, geometry
				, std::move( track ) } );
		m_ecs.createComponentData( entity
			, m_hitSound
			, soundSource );
	}

	void Bullet::resetData( Entity const & entity
		, SoundSource const * soundSource
		, TrackDataPtr track )
	{
		auto & data = m_ecs.getComponentData< BulletData >( entity
			, m_bullet ).getValue();
		*data.track = *track;
		data.status = BulletStatus::eFlying;
		m_ecs.getComponentData< SoundSource const * >( entity
			, m_hitSound ).setValue( soundSource );
	}

	String Bullet::toString( Entity const & entity )
	{
		auto stream = castor::makeStringStream();
		auto & data = m_ecs.getComponentData< BulletData >( entity
			, m_bullet ).getValue();
		stream << cuT( "Bullet(" ) << entity.getId() << cuT( ")" );
		stream << cuT( "\n Geometry: " ) << data.geometry->getName();
		stream << cuT( "\n Status: " ) << getName( data.status );
		stream << cuT( "\n Track: " ) << data.track->target.getId();
		return stream.str();
	}

	//*********************************************************************************************
}
