/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_WalkData_HPP___
#define ___EFO_WalkData_HPP___

#include "GridPathNode.hpp"

namespace orastus
{
	struct WalkData
	{
		EFO_API WalkData( GridPath const & p_path
			, Game const & p_game );

		Game const & m_game;
		GridPath m_path;
		GridPath::const_iterator m_current;
		Castor::Point3r m_destination;
	};
}

#endif
