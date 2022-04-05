#include "AnimationData.hpp"

#include <Castor3D/Scene/Animation/AnimatedObject.hpp>
#include <Castor3D/Scene/Animation/AnimatedObjectGroup.hpp>

namespace orastus
{
	AnimationData::AnimationData( castor3d::AnimatedObjectGroupSPtr p_animation
		, String const & p_name )
		: animation{ p_animation }
		, animName{ p_name }
	{
		auto & objects = animation->getObjects();
		auto & anim = objects.begin()->second->getAnimation( animName );
		total = anim.getAnimation().getLength();
	}
}
