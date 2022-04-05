#include "GameEngine/ECS/WalkData.hpp"

#include "GameEngine/Game.hpp"

namespace orastus
{
	WalkData::WalkData( GridPath const & p_path
		, Game const & p_game )
		: path{ p_path }
		, current{ path.begin() }
		, destination{ p_game.convert( castor::Point2i{ current->x, current->y } )
			+ castor::Point3f{ 0, p_game.getCellHeight(), 0 } }
	{
	}

	WalkData & WalkData::operator=( WalkData const & p_rhs )
	{
		path = p_rhs.path;
		current = path.begin() + std::distance( p_rhs.path.begin(), p_rhs.current );
		destination = p_rhs.destination;
		return *this;
	}
}
