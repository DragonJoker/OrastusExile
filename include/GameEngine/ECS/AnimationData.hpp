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
		EFO_API AnimationData( castor3d::AnimatedObjectGroupRPtr animation
			, String const & name );

		castor3d::AnimatedObjectGroupRPtr animation{};
		String animName;
		Milliseconds total;
		bool started{ false };
	};
}

#endif
