/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_GameEnginePrerequisites_HPP___
#define ___EFO_GameEnginePrerequisites_HPP___

#include <CastorUtilsPrerequisites.hpp>

#include <string>
#include <sstream>
#include <chrono>

#if !defined( _WIN32 )
#	define EFO_API
#else
#	if defined( GameEngine_EXPORTS )
#		define EFO_API __declspec( dllexport )
#	else
#		define EFO_API __declspec( dllimport )
#	endif
#endif

namespace orastus
{
	using String = Castor::String;
	using StringStream = Castor::StringStream;
	using Milliseconds = std::chrono::milliseconds;

	template< typename T >
	class Ability;

	class AbilitySet;
	class Resource;
	class Tower;

	using TowerPtr = std::unique_ptr< Tower >;
	using TowerRPtr = Tower *;
	using TowerArray = std::vector< TowerPtr >;
}

#endif
