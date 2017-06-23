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
#include <memory>
#include <vector>

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
	// General
	using Castor::String;
	using Castor::StringStream;
	using Clock = std::chrono::high_resolution_clock;
	using Milliseconds = std::chrono::milliseconds;

	// State
	class State;
	class StateMachine;
	using StateArray = std::vector< State >;
	using StateMachinePtr = std::shared_ptr< StateMachine >;

	// Map
	class Grid;
	struct GridCell;
	struct GridPathNode;
	using GridPath = std::vector< GridPathNode >;

	// ECS
	using ComponentId = uint64_t;
	class EntityId;
	class Entity;
	class Component;
	class Ecs;
	template< typename T >
	class ComponentData;
	class MapBlock;
	class Tower;
	class SplashTower;
	class Enemy;
	class Bullet;
	struct AnimationData;
	struct AttackData;
	struct TrackData;
	struct WalkData;
	using MapBlockPtr = std::shared_ptr< MapBlock >;
	using TowerPtr = std::shared_ptr< Tower >;
	using SplashTowerPtr = std::shared_ptr< SplashTower >;
	using EnemyPtr = std::shared_ptr< Enemy >;
	using BulletPtr = std::shared_ptr< Bullet >;
	using AnimationDataPtr = std::shared_ptr< AnimationData >;
	using AttackDataPtr = std::shared_ptr< AttackData >;
	using TrackDataPtr = std::shared_ptr< TrackData >;
	using WalkDataPtr = std::shared_ptr< WalkData >;
	using EntityList = std::vector< Entity >;

	// Game
	class Resource;
	class Game;

	// Helpers
	template< typename T >
	inline String ToString( T const & p_value )
	{
		StringStream l_stream;
		l_stream << p_value;
		return l_stream.str();
	}

	inline String ToString( Milliseconds const & p_value )
	{
		StringStream l_stream;
		l_stream << p_value.count() << " ms";
		return l_stream.str();
	}
}

#endif
