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
		EFO_API TrackData( Entity p_target
			, float p_speed
			, uint32_t p_damage );

		float m_speed;
		Entity m_target;
		uint32_t m_damage;
	};
}

#endif
