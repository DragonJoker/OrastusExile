#include "GameEngine/Ecs/SoundSource.hpp"

#include "GameEngine/Audio.hpp"
#include "GameEngine/Sound.hpp"

#include <AL/al.h>

namespace orastus
{
	SoundSource::SoundSource( Sound const * sound
		, bool looped
		, castor3d::SceneNode const * node )
		: m_node{ node }
	{
		if ( sound )
		{
			alGenSources( 1u, &m_source );
			checkALError( "OpenAL source creation" );
			alSourcei( m_source, AL_BUFFER, ALint( sound->getBuffer() ) );
			checkALError( "OpenAL source buffer binding" );

			if ( looped )
			{
				alSourcei( m_source, AL_LOOPING, AL_TRUE );
				checkALError( "OpenAL source loop setting" );
			}
#if !EFO_ManualAttenuation
			if ( m_node )
			{
				alSourcei( m_source, AL_SOURCE_RELATIVE, 0 );
				checkALError( "OpenAL source spatialisation setting" );
				auto position = m_node->getDerivedPosition();
				alSource3f( m_source, AL_POSITION, position->x, position->y, position->z );
				checkALError( "OpenAL source position setting" );
				alSourcef( m_source, AL_REFERENCE_DISTANCE, 100.0f );
				checkALError( "OpenAL source ref. distance setting" );
				alSourcef( m_source, AL_MAX_DISTANCE, 200.0f );
				checkALError( "OpenAL source max. distance setting" );
			}
#endif
		}
	}

	SoundSource::SoundSource()
		: SoundSource{ nullptr, false, nullptr }
	{
	}

	SoundSource::SoundSource( Sound const & sound
		, bool looped )
		: SoundSource{ &sound, looped, nullptr }
	{
	}

	SoundSource::SoundSource( Sound const & sound
		, bool looped
		, castor3d::SceneNode const & node )
		: SoundSource{ &sound, looped, &node }
	{
	}

	SoundSource::~SoundSource()
	{
		if ( m_source != AL_INVALID_VALUE )
		{
			alDeleteSources( 1u, &m_source );
			m_source = AL_INVALID_VALUE;
		}
	}

	void SoundSource::update( castor3d::SceneNode const & listener )const
	{
		if ( m_source != AL_INVALID_VALUE
			&& m_node )
		{
#if EFO_ManualAttenuation
			auto position = m_node->getDerivedPosition();
			auto listenerPosition = listener.getDerivedPosition();
			auto distance = float( castor::point::distance( position, listenerPosition ) );
			float refDistance = 100.0f;
			auto rollofFactor = 1.0f;
			auto gain = refDistance / ( refDistance + rollofFactor * ( distance - refDistance ) );
			alSourcef( m_source, AL_GAIN, gain );
#else
			auto position = m_node->getDerivedPosition();
			alSource3f( m_source, AL_POSITION, position->x, position->y, position->z );
#endif
		}
	}

	void SoundSource::play( castor3d::SceneNode const * node )const
	{
		if ( m_source != AL_INVALID_VALUE )
		{
			m_node = node;
			alSourcePlay( m_source );
			checkALError( "OpenAL source play" );
		}
	}

	void SoundSource::stop()const
	{
		if ( m_source != AL_INVALID_VALUE )
		{
			alSourceStop( m_source );
			checkALError( "OpenAL source stop" );
		}
	}
}
