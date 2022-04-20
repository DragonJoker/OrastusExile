#include "GameEngine/Sound.hpp"

#include "GameEngine/Audio.hpp"

#include <sndfile.h>

#include <algorithm>
#include <array>

CU_ImplementCUSmartPtr( orastus, Sound )

namespace orastus
{
	Sound::Sound( Type type
		, castor::Path const & filePath
		, uint32_t maxSources )
		: m_maxSources{ maxSources }
	{
		SF_INFO info{};
		SNDFILE * file = sf_open( filePath.c_str(), SFM_READ, &info );

		if ( !file )
		{
			throw std::runtime_error{ sf_strerror( nullptr ) };
		}


		if ( !sf_format_check( &info ) )
		{
			throw std::runtime_error{ sf_strerror( file ) };
		}

		std::vector< uint16_t > data;
		std::array< int16_t, 4096 > readBuf;
		sf_count_t readSize = 0;

		while ( ( readSize = sf_read_short( file, readBuf.data(), sf_count_t( readBuf.size() ) ) ) != 0 )
		{
			data.insert( data.end(), readBuf.begin(), readBuf.begin() + readSize );
		}

		sf_close( file );

		alGenBuffers( 1u, &m_buffer );
		checkALError( "OpenAL buffer creation" );
		alBufferData( m_buffer
			, info.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16
			, &data.front()
			, ALsizei( data.size() * sizeof( uint16_t ) )
			, info.samplerate );
		checkALError( "OpenAL buffer fill" );
	}

	Sound::~Sound()
	{
		if ( m_buffer != AL_INVALID_VALUE )
		{
			alDeleteBuffers( 1u, &m_buffer );
			checkALError( "OpenAL buffer destruction" );
		}
	}

	SoundSource const & Sound::createSource( bool looped )
	{
		if ( m_sources.size() < m_maxSources )
		{
			m_sources.emplace_back( std::make_unique< SoundSource >( *this, looped ), 0u );
		}

		return doGetMinUseSource();
	}

	SoundSource const & Sound::createSource( castor3d::SceneNode const & node
		, bool looped )
	{
		if ( m_sources.size() < m_maxSources )
		{
			m_sources.emplace_back( std::make_unique< SoundSource >( *this, looped, node ), 0u );
		}

		return doGetMinUseSource();
	}

	SoundSource const & Sound::doGetMinUseSource()
	{
		auto it = std::min_element( m_sources.begin()
			, m_sources.end()
			, []( std::pair< SoundSourcePtr, uint32_t > const & lhs
				, std::pair< SoundSourcePtr, uint32_t > const & rhs )
			{
				return lhs.second < rhs.second;
			} );
		++it->second;
		return *it->first;
	}
}
