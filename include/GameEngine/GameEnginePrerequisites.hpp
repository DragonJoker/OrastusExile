/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_GameEnginePrerequisites_HPP___
#define ___EFO_GameEnginePrerequisites_HPP___

#include <Castor3D/Castor3DPrerequisites.hpp>

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
	using castor::xchar;
	using castor::String;
	using castor::StringStream;
	using Clock = std::chrono::high_resolution_clock;
	using Milliseconds = castor::Milliseconds;

	// State
	class State;
	class StateMachine;
	using StateArray = std::vector< State >;
	using StateMachinePtr = std::unique_ptr< StateMachine >;

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
	class Target;
	class Tower;
	class SplashTower;
	class Enemy;
	class Bullet;
	struct AnimationData;
	struct AttackData;
	struct TrackData;
	struct WalkData;
	class SoundSource;
	using MapBlockPtr = std::shared_ptr< MapBlock >;
	using TargetPtr = std::shared_ptr< Target >;
	using TowerPtr = std::shared_ptr< Tower >;
	using SplashTowerPtr = std::shared_ptr< SplashTower >;
	using EnemyPtr = std::shared_ptr< Enemy >;
	using BulletPtr = std::shared_ptr< Bullet >;
	using AnimationDataPtr = std::unique_ptr< AnimationData >;
	using AttackDataPtr = std::unique_ptr< AttackData >;
	using TrackDataPtr = std::unique_ptr< TrackData >;
	using WalkDataPtr = std::unique_ptr< WalkData >;
	using SoundSourcePtr = std::unique_ptr< SoundSource >;
	using EntityList = std::vector< Entity >;

	// Game
	class Resource;
	class Game;
	class Audio;
	class Sound;

	CU_DeclareCUSmartPtr( orastus, Audio, EFO_API );
	CU_DeclareCUSmartPtr( orastus, Sound, EFO_API );

	// Helpers
	template< typename T >
	inline String toString( T const & value )
	{
		StringStream stream;
		stream << value;
		return stream.str();
	}

	inline String toString( Milliseconds const & value )
	{
		StringStream stream;
		stream << value.count() << " ms";
		return stream.str();
	}
}

#endif
