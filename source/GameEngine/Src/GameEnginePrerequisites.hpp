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

	using Clock = std::chrono::high_resolution_clock;
	using Milliseconds = std::chrono::milliseconds;

	class EntityId;
	class Entity;
	class Component;
	class Ecs;

	template< typename T >
	class ComponentData;

	class Resource;
	class Tower;
	class SplashTower;
	class Enemy;
	class Bullet;

	class Game;

	using TowerPtr = std::shared_ptr< Tower >;
	using SplashTowerPtr = std::shared_ptr< SplashTower >;
	using EnemyPtr = std::shared_ptr< Enemy >;
	using BulletPtr = std::shared_ptr< Bullet >;
}

#endif
