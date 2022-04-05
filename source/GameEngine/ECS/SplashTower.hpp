/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_SplashTower_HPP___
#define ___EFO_SplashTower_HPP___

#include "GameEnginePrerequisites.hpp"

namespace orastus
{
	/**
	*\brief
	*	The components used to describe a tower with splash damage.
	*/
	class SplashTower
	{
	public:
		/**
		*\brief
		*	Constructor, retrieves the components.
		*\param[in] p_ecs
		*	The ECS instance, from which the components are retrieved.
		*/
		EFO_API explicit SplashTower( Ecs & p_ecs );
		/**
		*\brief
		*	Creates the component data for a splash tower.
		*\param[in] p_entity
		*	The entity for the tower.
		*\param[in] p_cooldown
		*	The cooldown value.
		*\param[in] p_damage
		*	The damage value.
		*\param[in] p_range
		*	The range value.
		*\param[in] p_bulletSpeed
		*	The bullet speed value.
		*\param[in] p_splashDamage
		*	The splash damage value.
		*\param[in] p_splashRange
		*	The splash range value.
		*\param[in] p_requiredLevel
		*	The required level value.
		*\param[in] p_geometry
		*	The geometry value.
		*\param[in] p_animation
		*	The animation value.
		*\param[in] p_attack
		*	The attack data.
		*/
		EFO_API void createData( Entity const & p_entity
			, Milliseconds const & p_cooldown
			, uint32_t p_damage
			, float p_range
			, float p_bulletSpeed
			, uint32_t p_splashDamage
			, float p_splashRange
			, uint32_t p_requiredLevel
			, castor3d::GeometrySPtr p_geometry
			, AnimationDataPtr p_animation
			, AttackDataPtr p_attack );
		/**
		*\brief
		*	Returns a splash tower as a string.
		*\param[in] p_entity
		*	The entity to print.
		*/
		EFO_API String toString( Entity const & p_entity );

	private:
		Ecs & m_ecs;
		Component const & m_cooldown;
		Component const & m_damage;
		Component const & m_range;
		Component const & m_bulletSpeed;
		Component const & m_requiredLevel;
		Component const & m_state;
		Component const & m_splashDamage;
		Component const & m_splashRange;
		Component const & m_geometry;
		Component const & m_pickable;
		Component const & m_animation;
		Component const & m_attack;
	};
}

#endif
