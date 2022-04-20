/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Target_HPP___
#define ___EFO_Target_HPP___

#include "GameEngine/GridCell.hpp"
#include "GameEngine/Ecs/Entity.hpp"

namespace orastus
{
	struct TargetData
	{
		EFO_API TargetData( Entity entity
			, castor3d::GeometrySPtr geometry
			, GridCell cell );

		Entity entity;
		TargetStatus status;
		castor3d::GeometrySPtr geometry;
		GridCell cell;
	};
	/**
	*\brief
	*	The components used to describe an enemies target.
	*/
	class Target
	{
	public:
		/**
		*\brief
		*	Constructor, retrieves the components.
		*\param[in] ecs
		*	The ECS instance, from which the components are retrieved.
		*/
		EFO_API explicit Target( Ecs & ecs );
		/**
		*\brief
		*	Creates the component data for an enemies target.
		*\param[in] entity
		*	The entity for the target.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] cell
		*	The cell value.
		*/
		EFO_API void createData( Entity const & entity
			, castor3d::GeometrySPtr geometry
			, GridCell cell
			, SoundSource const * targetCapturedSound );
		/**
		*\brief
		*	Resets an enemy to initial state.
		*\param[in] entity
		*	The enemy entity.
		*\param[in] geometry
		*	The geometry value.
		*\param[in] cell
		*	The cell value.
		*/
		EFO_API void resetData( Entity const & entity
			, GridCell cell );
		/**
		*\brief
		*	Returns an enemy as a string.
		*\param[in] entity
		*	The entity to print.
		*/
		EFO_API String toString( Entity const & entity );

	private:
		Ecs & m_ecs;
		Component const & m_target;
		Component const & m_capturedSound;
	};
}

#endif
