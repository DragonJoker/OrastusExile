/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_TrackData_HPP___
#define ___EFO_TrackData_HPP___

#include "Entity.hpp"

namespace orastus
{
	struct TrackData
	{
		EFO_API TrackData( Entity target
			, float speed
			, uint32_t damage );

		float speed;
		Entity target;
		uint32_t damage;
	};
}

#endif
