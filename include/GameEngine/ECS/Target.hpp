/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Target_HPP___
#define ___EFO_Target_HPP___

#include "GameEngine/GameEnginePrerequisites.hpp"

namespace orastus
{
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
			, SoundSource targetCapturedSound );
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
			, castor3d::GeometrySPtr geometry
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
		Component const & m_geometry;
		Component const & m_state;
		Component const & m_status;
		Component const & m_cell;
		Component const & m_capturedSound;
	};
}

#endif
