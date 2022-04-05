#include "Grid.hpp"

namespace orastus
{
	Grid::Grid( uint32_t p_width, uint32_t p_height )
		: m_width{ p_width }
		, m_height{ p_height }
		, m_cells( m_width * m_height, GridCell{} )
	{
		uint32_t x = 0;
		uint32_t y = 0;

		for ( auto & l_cell : m_cells )
		{
			if ( x == m_width )
			{
				++y;
				x = 0;
			}

			l_cell.m_x = x;
			l_cell.m_y = y;
			++x;
		}
	}
}
