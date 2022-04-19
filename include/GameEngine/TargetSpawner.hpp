/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_TargetSpawner_HPP___
#define ___EFO_TargetSpawner_HPP___

#include "Ability.hpp"

#include <random>

namespace orastus
{
	class TargetSpawner
	{
	public:
		EFO_API TargetSpawner( Ecs & ecs
			, Game & game );

		EFO_API void reset();
		EFO_API void spawn( GridCell cell );
		/**
		*\brief
		*	Select an available target to capture.
		*\return
		*	The target entity.
		*/
		EFO_API Entity selectTarget();
		/**
		*\brief
		*	Frees a target being captured.
		*\param[in] target
		*	The target entity.
		*/
		EFO_API void freeTarget( Entity target );
		/**
		*\brief
		*	A target is being captured.
		*\param[in] target
		*	The target entity.
		*/
		EFO_API void targetBeingCaptured( Entity target );
		/**
		*\brief
		*	A target is successfully captured.
		*\param[in] target
		*	The target entity.
		*\return
		*	\p true if no more target remains.
		*/
		EFO_API bool targetCaptured( Entity target );

		uint32_t getLives()const
		{
			return m_count;
		}

	private:
		Ecs & m_ecs;
		Game & m_game;
		castor3d::MeshResPtr m_targetMesh;
		EntityList m_liveTargets;
		EntityList m_selectedTargets;
		EntityList m_targetsCache;
		uint32_t m_count{ 0 };
		std::uniform_real_distribution< float > m_distribution{ 0.0f, 360.0f };
	};
}

#endif
