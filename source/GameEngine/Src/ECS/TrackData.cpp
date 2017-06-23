#include "TrackData.hpp"

namespace orastus
{
	TrackData::TrackData( Entity p_target
		, float p_speed
		, uint32_t p_damage )
		: m_target{ p_target }
		, m_speed{ p_speed }
		, m_damage{ p_damage }
	{
	}
}
