#include "GameEngine/ECS/TowerCategory.hpp"

namespace orastus
{
	//*********************************************************************************************

	DirectTower::DirectTower()
		: TowerCategory{ TowerType::eDirect
			, GoldCost
			, Damage
			, Cooldown
			, Range
			, BulletSpeed
			, Ammo }
	{
	}

	//*********************************************************************************************

	SplashTower::SplashTower()
		: TowerCategory{ TowerType::eSplash
			, GoldCost
			, Damage
			, Cooldown
			, Range
			, BulletSpeed
			, Ammo }
	{
	}

	//*********************************************************************************************
}
