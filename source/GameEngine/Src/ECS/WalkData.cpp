#include "WalkData.hpp"

#include "Game.hpp"

namespace orastus
{
	WalkData::WalkData( GridPath const & p_path
		, Game const & p_game )
		: m_game{ p_game }
		, m_path{ p_path }
		, m_current{ m_path.begin() }
		, m_destination{ m_game.Convert( Castor::Point2i{ m_current->m_x, m_current->m_y } )
			+ Castor::Point3r{ 0, m_game.GetCellHeight(), 0 } }
	{
	}
}
