#include "TrackData.hpp"

namespace orastus
{
	TrackData::TrackData( Entity p_target
		, float p_speed
		, uint32_t p_damage )
		: speed{ p_speed }
		, target{ p_target }
		, damage{ p_damage }
	{
	}
}
