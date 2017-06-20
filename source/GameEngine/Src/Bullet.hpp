/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Bullet_HPP___
#define ___EFO_Bullet_HPP___

#include "GameEnginePrerequisites.hpp"

namespace orastus
{
	/**
	*\brief
	*	The components used to describe a bullet.
	*/
	class Bullet
	{
	public:
		/**
		*\brief
		*	Constructor, retrieves the components.
		*\param[in] p_game
		*	The game instance, from which the components are retrieved.
		*/
		EFO_API Bullet( Game & p_game );
		/**
		*\brief
		*	Creates the component data for a bullet.
		*\param[in] p_entity
		*	The entity for the bullet.
		*\param[in] p_speed
		*	The speed value.
		*\param[in] p_damage
		*	The damage value.
		*/
		void CreateData( Entity const & p_entity
			, float p_speed
			, uint32_t p_damage );

	private:
		Game & m_game;
		Component const & m_speed;
		Component const & m_damage;
	};
}

#endif
