/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Sound_HPP___
#define ___EFO_Sound_HPP___

#include "GameEngine/Ecs/SoundSource.hpp"

#include <AL/al.h>

namespace orastus
{
	/**
	*\brief
	*	Handles one sound.
	*/
	class Sound
	{
	public:
		enum class Type
		{
			eMusic,
			eSfx
		};

	public:
		EFO_API Sound( Type type
			, castor::Path const & file
			, uint32_t maxSources );
		EFO_API ~Sound();
		EFO_API SoundSource const & createSource( bool looped );
		EFO_API SoundSource const & createSource( castor3d::SceneNode const & node
			, bool looped );

		ALuint getBuffer()const
		{
			return m_buffer;
		}

	private:
		SoundSource const & doGetMinUseSource();

	private:
		ALuint m_buffer{ AL_INVALID_VALUE };
		std::vector< std::pair< SoundSourcePtr, uint32_t > > m_sources;
		uint32_t m_maxSources;
	};
}

#endif
