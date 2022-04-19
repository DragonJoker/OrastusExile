/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Sound_HPP___
#define ___EFO_Sound_HPP___

#include "GameEngine/GameEnginePrerequisites.hpp"

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
			, castor::Path const & file );
		EFO_API ~Sound();

		ALuint getBuffer()const
		{
			return m_buffer;
		}

	private:
		ALuint m_buffer{ AL_INVALID_VALUE };
	};
}

#endif
