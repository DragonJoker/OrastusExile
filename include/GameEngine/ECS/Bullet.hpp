/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Bullet_HPP___
#define ___EFO_Bullet_HPP___

#include "GameEngine/ECS/TrackData.hpp"

namespace orastus
{
	struct BulletData
	{
		EFO_API BulletData( Entity entity
			, AmmoType type
			, castor3d::GeometrySPtr geometry
			, TrackDataPtr track );

		Entity entity;
		AmmoType type;
		BulletStatus status;
		castor3d::GeometrySPtr geometry;
		TrackDataPtr track;
	};
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
		*\param[in] ecs
		*	The ECS instance, from which the components are retrieved.
		*/
		EFO_API explicit Bullet( Ecs & ecs );
		/**
		*\brief
		*	Creates the component data for a bullet.
		*\param[in] entity
		*	The entity for the bullet.
		*\param[in] type
		*	The ammunition type.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] track
		*	The track value.
		*/
		EFO_API void createData( Entity const & entity
			, AmmoType type
			, castor3d::GeometrySPtr geometry
			, SoundSource const * soundSource
			, TrackDataPtr track );
		/**
		*\brief
		*	Resets a bullet's component data to initial state.
		*\param[in] entity
		*	The entity for the bullet.
		*	The damage value.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] track
		*	The track value.
		*/
		EFO_API void resetData( Entity const & entity
			, SoundSource const * soundSource
			, TrackDataPtr track );
		/**
		*\brief
		*	Returns a bullet as a string.
		*\param[in] entity
		*	The entity to print.
		*/
		EFO_API String toString( Entity const & entity );

	private:
		Ecs & m_ecs;
		Component const & m_bullet;
		Component const & m_hitSound;
	};
}

#endif
