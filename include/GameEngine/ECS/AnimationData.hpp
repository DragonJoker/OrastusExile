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
		EFO_API AnimationData( castor3d::AnimatedObjectGroupSPtr p_animation
			, String const & p_name );

		castor3d::AnimatedObjectGroupSPtr animation;
		String animName;
		Milliseconds total;
		bool started{ false };
	};
}

#endif