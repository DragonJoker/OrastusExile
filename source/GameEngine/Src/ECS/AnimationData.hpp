/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_AnimationData_HPP___
#define ___EFO_AnimationData_HPP___

#include "Entity.hpp"

namespace orastus
{
	struct AnimationData
	{
		EFO_API AnimationData( Game & p_game
			, Castor3D::AnimatedObjectGroupSPtr p_animation
			, String const & p_name );

		Game & m_game;
		Castor3D::AnimatedObjectGroupSPtr m_animation;
		String m_animName;
		Milliseconds m_total;
		bool m_started{ false };
	};

	struct TowerAttackData
	{
		inline TowerAttackData( Milliseconds const & p_shootDelay )
			: m_shootDelay{ p_shootDelay }
		{
		}

		Milliseconds m_shootDelay;
		Entity m_target;
		bool m_shot{ false };
	};
}

#endif
