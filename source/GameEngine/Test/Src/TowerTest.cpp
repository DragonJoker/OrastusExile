/**
See licence file in root folder, MIT.txt
*/
#include "TowerTest.hpp"

#include <Tower.hpp>

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

		void TowerTest::DoRegisterTests()
		{
			DoRegisterTest( "TowerCreation", std::bind( &TowerTest::TowerCreation, this ) );
		}

		void TowerTest::TowerCreation()
		{
			CooldownAbility l_cooldown{ 100_ms };
			DamageAbility l_damage{ 1000 };
			RangeAbility l_range{ 0.56f };
			SpeedAbility l_speed{ 2.14f };
			AbilitySet l_set{ "TestAbilitySet", l_cooldown, l_damage, l_range, l_speed, 1 };
			Tower l_tower{ l_set };
			CT_EQUAL( l_tower.GetAbilities().GetCooldown(), l_set.GetCooldown() );
			CT_EQUAL( l_tower.GetAbilities().GetDamage(), l_set.GetDamage() );
			CT_EQUAL( l_tower.GetAbilities().GetRange(), l_set.GetRange() );
			CT_EQUAL( l_tower.GetAbilities().GetBulletSpeed(), l_set.GetBulletSpeed() );
			CT_EQUAL( l_tower.GetAbilities().GetRequiredLevel(), l_set.GetRequiredLevel() );
			CT_EQUAL( l_tower.GetAbilities().GetName(), l_set.GetName() );

			DamageAbility l_splashDamage{ 500 };
			RangeAbility l_splashRange{ 0.1f };
			AbilitySet l_splashSet{ "TestSplashAbilitySet", l_cooldown, l_damage, l_range, l_speed, l_splashDamage, l_splashRange, 2 };
			Tower l_splashTower{ l_splashSet };
			CT_EQUAL( l_splashTower.GetAbilities().GetCooldown(), l_splashSet.GetCooldown() );
			CT_EQUAL( l_splashTower.GetAbilities().GetDamage(), l_splashSet.GetDamage() );
			CT_EQUAL( l_splashTower.GetAbilities().GetRange(), l_splashSet.GetRange() );
			CT_EQUAL( l_splashTower.GetAbilities().GetBulletSpeed(), l_splashSet.GetBulletSpeed() );
			CT_EQUAL( l_splashTower.GetAbilities().GetSplashDamage(), l_splashSet.GetSplashDamage() );
			CT_EQUAL( l_splashTower.GetAbilities().GetSplashRange(), l_splashSet.GetSplashRange() );
			CT_EQUAL( l_splashTower.GetAbilities().GetRequiredLevel(), l_splashSet.GetRequiredLevel() );
			CT_EQUAL( l_splashTower.GetAbilities().GetName(), l_splashSet.GetName() );
		}
	}
}
