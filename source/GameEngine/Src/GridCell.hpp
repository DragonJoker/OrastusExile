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
			//! Grid path cell.
			ePath,
			//! Grid path target point.
			eTarget,
			//! Invalid cell.
			eInvalid,
		};
		/**
		*\brief
		*	Constructor.
		*\param[in] p_x
		*	The cell X coordinate.
		*\param[in] p_y
		*	The cell Y coordinate.
		*\param[in] p_state
		*	The cell initial state.
		*/
		inline GridCell( uint32_t p_x = 0u
			, uint32_t p_y = 0u
			, State p_state = State::eEmpty )
			: m_x{ p_x }
			, m_y{ p_y }
			, m_state{ p_state }
		{
		}
		/**
		*\brief
		*	Explicit conversion operator to bool.
		*\return
		*	\p false if the cell state is orastus::GridCell::State::eInvalid.
		*/
		inline explicit operator bool()const
		{
			return m_state != State::eInvalid;
		}
		//! The cell X coordinate.
		uint32_t m_x;
		//! The cell Y coordinate.
		uint32_t m_y;
		//! The cell state.
		State m_state;
		//! The entity at this cell.
		Entity m_entity;
	};
	//! A cell array.
	using CellArray = std::vector< GridCell >;
}

#endif
