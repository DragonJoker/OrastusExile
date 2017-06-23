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
		*\param[in] p_ecs
		*	The ECS instance, from which the components are retrieved.
		*/
		EFO_API explicit Bullet( Ecs & p_ecs );
		/**
		*\brief
		*	Creates the component data for a bullet.
		*\param[in] p_entity
		*	The entity for the bullet.
		*\param[in] p_speed
		*	The speed value.
		*\param[in] p_damage
		*	The damage value.
		*\param[in] p_geometry
		*	The geometry value.
		*/
		EFO_API void CreateData( Entity const & p_entity
			, float p_speed
			, uint32_t p_damage
			, Castor3D::GeometrySPtr p_geometry );
		/**
		*\brief
		*	Resets a bullet's component data to initial state.
		*\param[in] p_entity
		*	The entity for the bullet.
		*\param[in] p_speed
		*	The speed value.
		*\param[in] p_damage
		*	The damage value.
		*\param[in] p_geometry
		*	The geometry value.
		*/
		EFO_API void ResetData( Entity const & p_entity
			, float p_speed
			, uint32_t p_damage
			, Castor3D::GeometrySPtr p_geometry );
		/**
		*\brief
		*	Returns a bullet as a string.
		*\param[in] p_entity
		*	The entity to print.
		*/
		EFO_API String ToString( Entity const & p_entity );

	private:
		Ecs & m_ecs;
		Component const & m_speed;
		Component const & m_damage;
		Component const & m_geometry;
	};
}

#endif
