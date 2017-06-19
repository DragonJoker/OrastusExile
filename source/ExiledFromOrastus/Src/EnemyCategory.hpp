/**
See licence file in root folder, MIT.txt
*/
#pragma once

#include "Enemy.hpp"

namespace orastus
{
	namespace main
	{
		struct BaseEnemy
			: public Enemy::Category
		{
			BaseEnemy();
		};
	}
}
