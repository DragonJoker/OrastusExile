/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Bullet_HPP___
#define ___EFO_Bullet_HPP___

#include "GameEngine/GameEnginePrerequisites.hpp"

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
		*\param[in] p_geometry
		*	The geometry value.
		*\param[in] p_track
		*	The track value.
		*/
		EFO_API void createData( Entity const & p_entity
			, castor3d::GeometrySPtr p_geometry
			, TrackDataPtr p_track );
		/**
		*\brief
		*	Resets a bullet's component data to initial state.
		*\param[in] p_entity
		*	The entity for the bullet.
		*	The damage value.
		*\param[in] p_geometry
		*	The geometry value.
		*\param[in] p_track
		*	The track value.
		*/
		EFO_API void resetData( Entity const & p_entity
			, castor3d::GeometrySPtr p_geometry
			, TrackDataPtr p_track );
		/**
		*\brief
		*	Returns a bullet as a string.
		*\param[in] p_entity
		*	The entity to print.
		*/
		EFO_API String toString( Entity const & p_entity );

	private:
		Ecs & m_ecs;
		Component const & m_geometry;
		Component const & m_state;
		Component const & m_track;
	};
}

#endif
