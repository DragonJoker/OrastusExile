#include "GameEngine/ECS/TrackData.hpp"

namespace orastus
{
	TrackData::TrackData( Entity ptarget
		, float pspeed
		, uint32_t pdamage )
		: speed{ pspeed }
		, target{ ptarget }
		, damage{ pdamage }
	{
	}
}
