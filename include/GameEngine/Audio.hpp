/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Audio_HPP___
#define ___EFO_Audio_HPP___

#include "Sound.hpp"

#include <map>
#include <vector>

#include <al.h>
#include <alc.h>

#define EFO_ManualAttenuation 1

namespace orastus
{
	EFO_API bool checkALError( castor::String const & msg );
	/**
	*\brief
	*	The game audio handler.
	*/
	class Audio
	{
	public:
		/**
		*\brief
		*	Constructor.
		*/
		EFO_API Audio();
		/**
		*\brief
		*	Destructor.
		*/
		EFO_API ~Audio();
		/**
		*\brief
		*	Updates the listener.
		*/
		EFO_API void update( castor3d::SceneNode const & listener )const;
		/**
		*\brief
		*	Creates a sound.
		*/
		EFO_API Sound & addSound( Sound::Type type
			, castor::Path const & file
			, uint32_t maxSources );

	private:
		ALCdevice * m_device;
		ALvoid * m_data;
		ALCcontext * m_context;

	private:
		std::map< castor::Path, SoundUPtr > m_sounds;
	};
}

#endif
