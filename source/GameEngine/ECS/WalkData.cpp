#include "GameEngine/ECS/WalkData.hpp"

#include "GameEngine/Game.hpp"

namespace orastus
{
	WalkData::WalkData( GridPath const & ppath
		, Game const & game )
		: path{ ppath }
		, current{ path.begin() }
		, destination{ game.convert( castor::Point2i{ current->x, current->y } )
			+ castor::Point3f{ 0, game.getCellHeight(), 0 } }
	{
	}

	WalkData & WalkData::operator=( WalkData const & rhs )
	{
		path = rhs.path;
		current = path.begin() + std::distance( rhs.path.begin(), rhs.current );
		destination = rhs.destination;
		return *this;
	}
}
