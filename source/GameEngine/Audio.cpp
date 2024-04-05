#include "GameEngine/Audio.hpp"

CU_ImplementSmartPtr( orastus, Audio )

namespace orastus
{
	bool checkALError( castor::String const & msg )
	{
		auto result = alGetError();

		if ( result != AL_NO_ERROR )
		{
			castor3d::log::error << msg << std::endl;
		}

		return result == AL_NO_ERROR;
	}

	Audio::Audio()
		: m_device{ alcOpenDevice( nullptr ) }
		, m_context{ alcCreateContext( m_device, nullptr ) }
	{
		alcMakeContextCurrent( m_context );
		checkALError( "OpenAL context creation" );
	}

	Audio::~Audio()
	{
		m_sounds.clear();
		alcMakeContextCurrent( nullptr );
		alcDestroyContext( m_context );
		alcCloseDevice( m_device );
	}

	void Audio::update( castor3d::SceneNode const & listener )const
	{
#if !EFO_ManualAttenuation
		auto position = listener.getDerivedPosition();
		alListener3f( AL_POSITION, position->x, position->y, position->z );

		auto orientation = listener.getDerivedOrientation();
		castor::Point3f up{ 0.0f, 1.0f, 0.0f };
		castor::Point3f at{ 0.0f, 0.0f, -1.0f };
		orientation.transform( up, up );
		orientation.transform( at, at );
		std::array< float, 6u > orient{ at->x, at->y, at->z, up->x, up->y, up->z };
		alListenerfv( AL_ORIENTATION, orient.data() );
#endif
	}

	Sound & Audio::addSound( Sound::Type type
		, castor::Path const & file
		, uint32_t maxSources )
	{
		auto ires = m_sounds.emplace( file, nullptr );
		
		if ( ires.second )
		{
			ires.first->second = castor::makeUnique< Sound >( type, file, maxSources );
		}

		return *ires.first->second;
	}
}
