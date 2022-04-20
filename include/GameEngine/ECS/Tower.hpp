/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Tower_HPP___
#define ___EFO_Tower_HPP___

#include "GameEngine/ECS/AnimationData.hpp"
#include "GameEngine/ECS/AttackData.hpp"

namespace orastus
{
	struct TowerData
	{
		EFO_API TowerData( Entity entity
			, TowerType type
			, AttackDataPtr attack
			, castor3d::GeometrySPtr geometry
			, float range
			, float bulletSpeed
			, uint32_t damage
			, Milliseconds cooldown
			, AnimationDataPtr anim );
		EFO_API TowerData( Entity entity
			, TowerType type
			, AttackDataPtr attack
			, castor3d::GeometrySPtr geometry
			, float range
			, float bulletSpeed
			, uint32_t damage
			, Milliseconds cooldown
			, Milliseconds timeout );

		Entity entity;
		TowerType type;
		TowerStatus status;
		AttackDataPtr attack;
		castor3d::GeometrySPtr geometry;
		float range;
		float bulletSpeed;
		uint32_t damage;
		Milliseconds cooldown;
		AnimationDataPtr anim;
		Milliseconds timeout;
	};
	/**
	*\brief
	*	The components used to describe a tower.
	*/
	class Tower
	{
	public:
		/**
		*\brief
		*	Constructor, retrieves the components.
		*\param[in] ecs
		*	The ECS instance, from which the components are retrieved.
		*/
		EFO_API explicit Tower( Ecs & ecs );
		/**
		*\brief
		*	Creates the component data for a tower.
		*\param[in] entity
		*	The entity for the tower.
		*\param[in] type
		*	The tower type.
		*\param[in] cooldown
		*	The cooldown value.
		*\param[in] damage
		*	The damage value.
		*\param[in] range
		*	The range value.
		*\param[in] bulletSpeed
		*	The bullet speed value.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] animation
		*	The animation value.
		*\param[in] attack
		*	The attack data.
		*\param[in] shootSound
		*	The shoot sound source.
		*/
		EFO_API void createData( Entity const & entity
			, TowerType type
			, Milliseconds const & cooldown
			, uint32_t damage
			, float range
			, float bulletSpeed
			, castor3d::GeometrySPtr geometry
			, AnimationDataPtr animation
			, AttackDataPtr attack
			, SoundSource const * shootSound );
		/**
		*\brief
		*	Returns a splash tower as a string.
		*\param[in] entity
		*	The entity to print.
		*/
		EFO_API String toString( Entity const & entity );

	private:
		Ecs & m_ecs;
		Component const & m_tower;
		Component const & m_geometry;
		Component const & m_pickable;
		Component const & m_soundSource;
	};
}

#endif
