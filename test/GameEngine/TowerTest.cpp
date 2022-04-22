/**
See licence file in root folder, MIT.txt
*/
#include "TowerTest.hpp"

#include <GameEngine/GridCell.hpp>
#include <GameEngine/ECS/Ecs.hpp>
#include <GameEngine/ECS/SoundSource.hpp>
#include <GameEngine/ECS/Tower.hpp>

namespace orastus
{
	namespace test
	{
		TowerTest::TowerTest()
			: Testing::TestCase{ "TowerTest" }
		{
		}

		TowerTest::~TowerTest()
		{
		}

		void TowerTest::doRegisterTests()
		{
			doRegisterTest( "TowerTest::Creation", std::bind( &TowerTest::Creation, this ) );
		}

		void TowerTest::Creation()
		{
			Ecs ecs;
			GridCell cell{ 0u, 0u, GridCell::State::eInvalid };
			auto tower = ecs.createTower( std::make_unique< DirectTower >(), cell, nullptr, nullptr, nullptr, {} );
			auto & towerData = ecs.getComponentData< TowerData >( tower, ecs.getComponent( Ecs::TowerStateComponent ) ).getValue();
			CT_EQUAL( towerData.category->getType(), TowerType::eDirect );
			CT_EQUAL( towerData.category->getCooldown(), DirectTower::Cooldown );
			CT_EQUAL( towerData.category->getDamage(), DirectTower::Damage );
			CT_EQUAL( towerData.category->getRange(), DirectTower::Range );
			CT_EQUAL( towerData.category->getBulletSpeed(), DirectTower::BulletSpeed );
			CT_EQUAL( towerData.timeout, 0_ms );

			auto splashTower = ecs.createTower( std::make_unique< SplashTower >(), cell, nullptr, nullptr, nullptr, {} );
			auto & splashTowerData = ecs.getComponentData< TowerData >( splashTower, ecs.getComponent( Ecs::TowerStateComponent ) ).getValue();
			CT_EQUAL( splashTowerData.category->getType(), TowerType::eSplash );
			CT_EQUAL( splashTowerData.category->getCooldown(), SplashTower::Cooldown );
			CT_EQUAL( splashTowerData.category->getDamage(), SplashTower::Damage );
			CT_EQUAL( splashTowerData.category->getRange(), SplashTower::Range );
			CT_EQUAL( splashTowerData.category->getBulletSpeed(), SplashTower::BulletSpeed );
			CT_EQUAL( splashTowerData.timeout, 0_ms );
		}
	}
}
