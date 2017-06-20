/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_GridPathNode_HPP___
#define ___EFO_GridPathNode_HPP___

#include "GameEnginePrerequisites.hpp"

namespace orastus
{
	/**
	*\brief
	*	A path node in the grid.
	*/
	struct GridPathNode
	{
		//! The X coordinate.
		uint32_t m_x;
		//! The Y coordinate.
		uint32_t m_y;
	};
	//! The path nodes describing a path.
	using GridPath = std::vector< GridPathNode >;
}

#endif
