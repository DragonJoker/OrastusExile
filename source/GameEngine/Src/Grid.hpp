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
		*\param[in] p_width, p_height
		*	The grid dimensions.
		*/
		EFO_API Grid( uint32_t p_width = 32u
			, uint32_t p_height = 32u );
		/**
		*\return
		*	An iterator to the beginning of the cells.
		*/
		inline auto begin()const
		{
			return m_cells.begin();
		}
		/**
		*\return
		*	An iterator to the beginning of the cells.
		*/
		inline auto begin()
		{
			return m_cells.begin();
		}
		/**
		*\return
		*	An iterator to the end of the cells.
		*/
		inline auto end()const
		{
			return m_cells.end();
		}
		/**
		*\return
		*	An iterator to the end of the cells.
		*/
		inline auto end()
		{
			return m_cells.end();
		}
		/**
		*\return
		*	The grid width.
		*/
		inline auto GetWidth()const
		{
			return m_width;
		}
		/**
		*\return
		*	The grid width.
		*/
		inline auto GetHeight()const
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
		inline auto & operator()( uint32_t x, uint32_t y )const
		{
			REQUIRE( x < m_width && y < m_height );
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
		inline auto & operator()( uint32_t x, uint32_t y )
		{
			REQUIRE( x < m_width && y < m_height );
			return m_cells[x * m_height + y];
		}

	private:
		uint32_t m_width;
		uint32_t m_height;
		CellArray m_cells;
	};
}

#endif
