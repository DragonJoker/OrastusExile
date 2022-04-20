#include "GameEngine/ECS/TowerCategory.hpp"

namespace orastus
{
	//*********************************************************************************************

	DirectTower::DirectTower()
		: TowerCategory{ TowerType::eDirect
			, Damage
			, Cooldown
			, Range
			, BulletSpeed }
	{
	}

	//*********************************************************************************************

	SplashTower::SplashTower()
		: TowerCategory{ TowerType::eSplash
			, Damage
			, Cooldown
			, Range
			, BulletSpeed }
	{
	}

	//*********************************************************************************************
}
