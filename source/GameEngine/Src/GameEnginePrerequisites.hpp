/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_GameEnginePrerequisites_HPP___
#define ___EFO_GameEnginePrerequisites_HPP___

#include <Castor3DPrerequisites.hpp>

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
	using Castor::String;
	using Castor::StringStream;

	using Milliseconds = std::chrono::milliseconds;

	class EntityId;
	class Entity;
	class Component;
	class Game;

	template< typename T >
	class ComponentData;

	class Resource;
	class Tower;
	class SplashTower;
	class Enemy;
	class Bullet;

	using TowerPtr = std::unique_ptr< Tower >;
	using SplashTowerPtr = std::unique_ptr< SplashTower >;
	using EnemyPtr = std::unique_ptr< Enemy >;
	using BulletPtr = std::unique_ptr< Bullet >;
}

#endif
