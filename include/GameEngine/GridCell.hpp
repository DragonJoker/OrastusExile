/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_GridCell_HPP___
#define ___EFO_GridCell_HPP___

#include "Ecs/Entity.hpp"

namespace orastus
{
	/**
	*\brief
	*	A cell inside the grid.
	*/
	struct GridCell
	{
		/**
		*\brief
		*	Cell states enumeration.
		*/
		enum class State
			: uint8_t
		{
			//! Empty cell.
			eEmpty,
			//! Grid path start point.
			eStart,
			//! Cell with a tower.
			eTower,
			//! Grid path cells.
			ePathStraightSN,
			ePathStraightEW,
			ePathTurnNE,
			ePathTurnES,
			ePathTurnSW,
			ePathTurnWN,
			ePathArea,
			//! Grid path target point.
			eTarget,
			//! Invalid cell.
			eInvalid,
		};
		/**
		*\brief
		*	Constructor.
		*\param[in] px
		*	The cell X coordinate.
		*\param[in] py
		*	The cell Y coordinate.
		*\param[in] pstate
		*	The cell initial state.
		*/
		explicit GridCell( uint32_t px = 0u
			, uint32_t py = 0u
			, State pstate = State::eEmpty )
			: x{ px }
			, y{ py }
			, state{ pstate }
		{
		}
		/**
		*\brief
		*	Explicit conversion operator to bool.
		*\return
		*	\p false if the cell state is orastus::GridCell::State::eInvalid.
		*/
		explicit operator bool()const
		{
			return state != State::eInvalid;
		}
		//! The cell X coordinate.
		uint32_t x;
		//! The cell Y coordinate.
		uint32_t y;
		//! The cell state.
		State state;
		//! The entity at this cell.
		Entity entity;
	};
	//! A cell array.
	using CellArray = std::vector< GridCell >;
}

#endif
