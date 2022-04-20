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
	/**
	*\brief
	*	Tower types enumeration.
	*/
	enum class TowerType
	{
		//! Direct damage tower.
		eDirect,
		//! Splash damage tower.
		eSplash,
	};
	/**
	*\brief
	*	Retrieves the name of the given tower type.
	*\param[in] type
	*	The tower type.
	*\return
	*	The name.
	*/
	EFO_API String getName( TowerType type );
	/**
	*\brief
	*	Tower states enumeration.
	*/
	enum class TowerStatus
	{
		//! No executor for the tower.
		eEmpty,
		//! The tower is looking for an enemy to shoot at.
		eIdle,
		//! The tower is shooting an enemy.
		eShooting,
	};
	/**
	*\brief
	*	Retrieves the name of the given tower state.
	*\param[in] state
	*	The tower state.
	*\return
	*	The name.
	*/
	EFO_API String getName( TowerStatus state );
	/**
	*\brief
	*	Enemy states enumeration.
	*/
	enum class EnemyStatus
	{
		//! The enemy is spawning.
		eSpawning,
		//! The enemy is walking.
		eWalking,
		//! The enemy is frozen.
		eFrozen,
		//! The enemy is slown down.
		eSlown,
		//! The enemy is dying (id est: playing the death animation)
		eDying,
		//! The enemy is dead.
		eDead,
		//! The enemy is selecting a target.
		eSelecting,
		//! The enemy is waiting for a target.
		eWaiting,
		//! The enemy is capturing a target.
		eCapturing,
		//! The enemy is escaping the map.
		eEscaping,
		//! The enemy has escaped the map.
		eEscaped,
	};
	/**
	*\brief
	*	Retrieves the name of the given enemy state.
	*\param[in] state
	*	The tower state.
	*\return
	*	The name.
	*/
	EFO_API String getName( EnemyStatus state );
	/**
	*\brief
	*	Target states enumeration.
	*/
	enum class TargetStatus
	{
		//! The target is idle.
		eIdle,
		//! The target is being captured.
		eCapturing,
		//! The target is captured.
		eCaptured,
	};
	/**
	*\brief
	*	Retrieves the name of the given enemy state.
	*\param[in] state
	*	The tower state.
	*\return
	*	The name.
	*/
	EFO_API String getName( TargetStatus state );
	/**
	*\brief
	*	Bullet states enumeration.
	*/
	enum class BulletStatus
	{
		//! The bullet is idle.
		eIdle,
		//! The bullet is flying to its target.
		eFlying,
	};
	/**
	*\brief
	*	Retrieves the name of the given enemy state.
	*\param[in] state
	*	The tower state.
	*\return
	*	The name.
	*/
	EFO_API String getName( BulletStatus state );

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
	class BaseComponentData;
	class Ecs;
	template< typename T >
	class ComponentData;
	class MapBlock;
	class Target;
	class Tower;
	class Enemy;
	class Bullet;
	class SoundSource;

	struct AnimationData;
	struct AttackData;
	struct BulletData;
	struct EnemyData;
	struct TargetData;
	struct TowerData;
	struct TrackData;
	struct WalkData;
	struct EnemyCategory;
	struct TowerCategory;

	using MapBlockPtr = std::shared_ptr< MapBlock >;
	using TargetPtr = std::shared_ptr< Target >;
	using TowerPtr = std::shared_ptr< Tower >;
	using EnemyPtr = std::shared_ptr< Enemy >;
	using BulletPtr = std::shared_ptr< Bullet >;
	using AnimationDataPtr = std::unique_ptr< AnimationData >;
	using AttackDataPtr = std::unique_ptr< AttackData >;
	using TrackDataPtr = std::unique_ptr< TrackData >;
	using WalkDataPtr = std::unique_ptr< WalkData >;
	using SoundSourcePtr = std::unique_ptr< SoundSource >;
	using EntityList = std::vector< Entity >;
	using EnemyCategoryPtr = std::unique_ptr< EnemyCategory >;
	using TowerCategoryPtr = std::unique_ptr< TowerCategory >;

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
