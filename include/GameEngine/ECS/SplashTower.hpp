/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_SplashTower_HPP___
#define ___EFO_SplashTower_HPP___

#include "GameEngine/GameEnginePrerequisites.hpp"

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
		*\param[in] ecs
		*	The ECS instance, from which the components are retrieved.
		*/
		EFO_API explicit SplashTower( Ecs & ecs );
		/**
		*\brief
		*	Creates the component data for a splash tower.
		*\param[in] entity
		*	The entity for the tower.
		*\param[in] cooldown
		*	The cooldown value.
		*\param[in] damage
		*	The damage value.
		*\param[in] range
		*	The range value.
		*\param[in] bulletSpeed
		*	The bullet speed value.
		*\param[in] splashDamage
		*	The splash damage value.
		*\param[in] splashRange
		*	The splash range value.
		*\param[in] requiredLevel
		*	The required level value.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] animation
		*	The animation value.
		*\param[in] attack
		*	The attack data.
		*/
		EFO_API void createData( Entity const & entity
			, Milliseconds const & cooldown
			, uint32_t damage
			, float range
			, float bulletSpeed
			, uint32_t splashDamage
			, float splashRange
			, uint32_t requiredLevel
			, castor3d::GeometrySPtr geometry
			, AnimationDataPtr animation
			, AttackDataPtr attack
			, SoundSource shootSound
			, Sound const * hitSound );
		/**
		*\brief
		*	Returns a splash tower as a string.
		*\param[in] entity
		*	The entity to print.
		*/
		EFO_API String toString( Entity const & entity );

	private:
		Ecs & m_ecs;
		Component const & m_cooldown;
		Component const & m_timeout;
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
		Component const & m_shootSound;
		Component const & m_hitSound;
	};
}

#endif
