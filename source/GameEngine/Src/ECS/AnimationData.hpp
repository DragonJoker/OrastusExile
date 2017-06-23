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
		EFO_API AnimationData( Castor3D::AnimatedObjectGroupSPtr p_animation
			, String const & p_name );

		Castor3D::AnimatedObjectGroupSPtr m_animation;
		String m_animName;
		Milliseconds m_total;
		bool m_started{ false };
	};
}

#endif
