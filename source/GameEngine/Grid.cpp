#include "GameEngine/Grid.hpp"

namespace orastus
{
	Grid::Grid( uint32_t width, uint32_t height )
		: m_width{ width }
		, m_height{ height }
		, m_cells( m_width * m_height, GridCell{} )
	{
		uint32_t x = 0;
		uint32_t y = 0;

		for ( auto & cell : m_cells )
		{
			if ( x == m_width )
			{
				++y;
				x = 0;
			}

			cell.x = x;
			cell.y = y;
			++x;
		}
	}
}
