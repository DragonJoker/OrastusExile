/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Grid_HPP___
#define ___EFO_Grid_HPP___

#include "GridCell.hpp"
#include "GridPathNode.hpp"

namespace orastus
{
	/**
	*\brief
	*	The grid describing the map.
	*/
	class Grid
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] width, height
		*	The grid dimensions.
		*/
		EFO_API Grid( uint32_t width = 32u
			, uint32_t height = 32u );
		/**
		*\return
		*	An iterator to the beginning of the cells.
		*/
		auto begin()const
		{
			return m_cells.begin();
		}
		/**
		*\return
		*	An iterator to the beginning of the cells.
		*/
		auto begin()
		{
			return m_cells.begin();
		}
		/**
		*\return
		*	An iterator to the end of the cells.
		*/
		auto end()const
		{
			return m_cells.end();
		}
		/**
		*\return
		*	An iterator to the end of the cells.
		*/
		auto end()
		{
			return m_cells.end();
		}
		/**
		*\return
		*	The grid width.
		*/
		auto getWidth()const
		{
			return m_width;
		}
		/**
		*\return
		*	The grid width.
		*/
		auto getHeight()const
		{
			return m_height;
		}
		/**
		*\brief
		*	Matrix access operator.
		*\param[in] x, y
		*	The coordinates.
		*\return
		*	The Cell at given coordinates.
		*/
		auto & operator()( uint32_t x, uint32_t y )const
		{
			CU_Require( x < m_width && y < m_height );
			return m_cells[x * m_height + y];
		}
		/**
		*\brief
		*	Matrix access operator.
		*\param[in] x, y
		*	The coordinates.
		*\return
		*	The Cell at given coordinates.
		*/
		auto & operator()( uint32_t x, uint32_t y )
		{
			CU_Require( x < m_width && y < m_height );
			return m_cells[x * m_height + y];
		}

	private:
		uint32_t m_width;
		uint32_t m_height;
		CellArray m_cells;
	};
}

#endif
