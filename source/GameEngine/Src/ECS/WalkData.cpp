#include "WalkData.hpp"

#include "Game.hpp"

namespace orastus
{
	WalkData::WalkData( GridPath const & p_path
		, Game const & p_game )
		: m_path{ p_path }
		, m_current{ m_path.begin() }
		, m_destination{ p_game.Convert( Castor::Point2i{ m_current->m_x, m_current->m_y } )
			+ Castor::Point3r{ 0, p_game.GetCellHeight(), 0 } }
	{
	}

	WalkData & WalkData::operator=( WalkData const & p_rhs )
	{
		m_path = p_rhs.m_path;
		m_current = m_path.begin() + std::distance( p_rhs.m_path.begin(), p_rhs.m_current );
		m_destination = p_rhs.m_destination;
		return *this;
	}
}
