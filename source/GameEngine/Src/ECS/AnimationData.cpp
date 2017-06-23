#include "AnimationData.hpp"

#include <Scene/Animation/AnimatedObject.hpp>
#include <Scene/Animation/AnimatedObjectGroup.hpp>

namespace orastus
{
	AnimationData::AnimationData( Game & p_game
		, Castor3D::AnimatedObjectGroupSPtr p_animation
		, String const & p_name )
		: m_game{ p_game }
		, m_animation{ p_animation }
		, m_animName{ p_name }
	{
		auto & l_objects = m_animation->GetObjects();
		auto & l_animation = l_objects.begin()->second->GetAnimation( m_animName );
		m_total = l_animation.GetAnimation().GetLength();
	}
}
