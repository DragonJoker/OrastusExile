#include "GameEngine/ECS/AnimationData.hpp"

#include <Castor3D/Scene/Animation/AnimatedObject.hpp>
#include <Castor3D/Scene/Animation/AnimatedObjectGroup.hpp>

namespace orastus
{
	AnimationData::AnimationData( castor3d::AnimatedObjectGroupRPtr panimation
		, String const & name )
		: animation{ panimation }
		, animName{ name }
	{
		auto & objects = animation->getObjects();
		auto & anim = objects.begin()->second->getAnimation( animName );
		total = anim.getAnimation().getLength();
	}
}
