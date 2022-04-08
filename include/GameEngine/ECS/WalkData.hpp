/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_WalkData_HPP___
#define ___EFO_WalkData_HPP___

#include "GameEngine/GridPathNode.hpp"

namespace orastus
{
	struct WalkData
	{
		EFO_API WalkData( GridPath const & path
			, Game const & game );
		EFO_API WalkData & operator=( WalkData const & rhs );

		GridPath path;
		GridPath::const_iterator current;
		castor::Point3f destination;
	};
}

#endif
