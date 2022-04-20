/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_SoundSource_HPP___
#define ___EFO_SoundSource_HPP___

#include "GameEngine/GameEnginePrerequisites.hpp"

namespace orastus
{
	/**
	*\brief
	*	Handles one sound source.
	*/
	class SoundSource
	{
	private:
		EFO_API SoundSource( Sound const * sound
			, bool looped
			, castor3d::SceneNode const * node );

	public:
		EFO_API SoundSource();
		EFO_API SoundSource( Sound const & sound
			, bool looped );
		EFO_API SoundSource( Sound const & sound
			, bool looped
			, castor3d::SceneNode const & node );
		EFO_API SoundSource( SoundSource const & rhs ) = delete;
		EFO_API SoundSource( SoundSource && rhs ) = delete;
		EFO_API SoundSource & operator=( SoundSource const & rhs ) = delete;
		EFO_API SoundSource & operator=( SoundSource && rhs ) = delete;
		EFO_API ~SoundSource();
		/**
		*\brief
		*	Updates the source position.
		*/
		EFO_API void update( castor3d::SceneNode const & listener )const;
		/**
		*\brief
		*	Plays the sound.
		*/
		EFO_API void play( castor3d::SceneNode const * node = nullptr )const;
		/**
		*\brief
		*	Stops playing the sound.
		*/
		EFO_API void stop()const;

	private:
		mutable castor3d::SceneNode const * m_node;
		unsigned int m_source{};
	};
}

#endif
