/**
See licence file in root folder, MIT.txt
*/
#include "TowerTest.hpp"

#include <GameEngine/ECS/Ecs.hpp>
#include <GameEngine/ECS/SoundSource.hpp>
#include <GameEngine/ECS/SplashTower.hpp>
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
			Milliseconds cooldown{ 100_ms };
			uint32_t damage{ 1000u };
			float range{ 0.56f };
			float speed{ 2.14f };
			auto tower = ecs.createTower( TowerType::eShortRange, cooldown, damage, range, speed, nullptr, nullptr, nullptr, {} );
			auto & towerData = ecs.getComponentData< TowerData >( tower, ecs.getComponent( Ecs::TowerStateComponent ) ).getValue();
			CT_EQUAL( towerData.cooldown, cooldown );
			CT_EQUAL( towerData.timeout, 0_ms );
			CT_EQUAL( towerData.damage, damage );
			CT_EQUAL( towerData.range, range );
			CT_EQUAL( towerData.bulletSpeed, speed );

			uint32_t splashDamage{ 500u };
			float splashRange{ 0.1f };
			auto splashTower = ecs.createTower( TowerType::eShortRange, cooldown, damage, range, speed, splashDamage, splashRange, nullptr, nullptr, nullptr, {} );
			auto & splashTowerData = ecs.getComponentData< SplashTowerData >( splashTower, ecs.getComponent( Ecs::SplashTowerStateComponent ) ).getValue();
			CT_EQUAL( splashTowerData.cooldown, cooldown );
			CT_EQUAL( splashTowerData.timeout, 0_ms );
			CT_EQUAL( splashTowerData.damage, damage );
			CT_EQUAL( splashTowerData.range, range );
			CT_EQUAL( splashTowerData.bulletSpeed, speed );
			CT_EQUAL( splashTowerData.splashDamage, splashDamage );
			CT_EQUAL( splashTowerData.splashRange, splashRange );
		}
	}
}
