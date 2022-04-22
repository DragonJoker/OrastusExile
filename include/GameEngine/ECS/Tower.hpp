/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Tower_HPP___
#define ___EFO_Tower_HPP___

#include "GameEngine/ECS/AnimationData.hpp"
#include "GameEngine/ECS/AttackData.hpp"
#include "GameEngine/ECS/TowerCategory.hpp"

namespace orastus
{
	struct TowerData
	{
		EFO_API TowerData( Entity entity
			, TowerCategoryPtr category
			, AttackDataPtr attack
			, castor3d::GeometrySPtr geometry
			, AnimationDataPtr anim );
		EFO_API TowerData( Entity entity
			, TowerCategoryPtr category
			, AttackDataPtr attack
			, castor3d::GeometrySPtr geometry
			, Milliseconds timeout );

		Entity entity;
		TowerCategoryPtr category;
		TowerStatus status;
		AttackDataPtr attack;
		castor3d::GeometrySPtr geometry;
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
		*\param[in] category
		*	The tower category.
		*\param[in] cell
		*	The tower cell.
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
			, TowerCategoryPtr category
			, GridCell & cell
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
		Component const & m_cell;
		Component const & m_geometry;
		Component const & m_pickable;
		Component const & m_soundSource;
	};
}

#endif
