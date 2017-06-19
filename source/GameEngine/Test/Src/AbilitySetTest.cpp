/**
See licence file in root folder, MIT.txt
*/
#include "AbilitySetTest.hpp"

#include <AbilitySet.hpp>

namespace orastus
{
	namespace test
	{
		AbilitySetTest::AbilitySetTest()
			: Testing::TestCase{ "AbilitySetTest" }
		{
		}

		AbilitySetTest::~AbilitySetTest()
		{
		}

		void AbilitySetTest::DoRegisterTests()
		{
			DoRegisterTest( "AbilitySetCreation", std::bind( &AbilitySetTest::AbilitySetCreation, this ) );
		}

		void AbilitySetTest::AbilitySetCreation()
		{
			CooldownAbility l_cooldown{ 100_ms };
			DamageAbility l_damage{ 1000 };
			RangeAbility l_range{ 0.56f };
			SpeedAbility l_speed{ 2.14f };
			AbilitySet l_set{ "TestAbilitySet", l_cooldown, l_damage, l_range, l_speed, 1 };
			CT_EQUAL( l_set.GetCooldown(), l_cooldown.GetValue() );
			CT_EQUAL( l_set.GetDamage(), l_damage.GetValue() );
			CT_EQUAL( l_set.GetRange(), l_range.GetValue() );
			CT_EQUAL( l_set.GetBulletSpeed(), l_speed.GetValue() );
			CT_EQUAL( l_set.GetRequiredLevel(), 1 );
			CT_EQUAL( l_set.GetName(), "TestAbilitySet" );

			DamageAbility l_splashDamage{ 500 };
			RangeAbility l_splashRange{ 0.1f };
			AbilitySet l_splashSet{ "TestSplashAbilitySet", l_cooldown, l_damage, l_range, l_speed, l_splashDamage, l_splashRange, 2 };
			CT_EQUAL( l_splashSet.GetCooldown(), l_cooldown.GetValue() );
			CT_EQUAL( l_splashSet.GetDamage(), l_damage.GetValue() );
			CT_EQUAL( l_splashSet.GetRange(), l_range.GetValue() );
			CT_EQUAL( l_splashSet.GetBulletSpeed(), l_speed.GetValue() );
			CT_EQUAL( l_splashSet.GetSplashDamage(), l_splashDamage.GetValue() );
			CT_EQUAL( l_splashSet.GetSplashRange(), l_splashRange.GetValue() );
			CT_EQUAL( l_splashSet.GetRequiredLevel(), 2 );
			CT_EQUAL( l_splashSet.GetName(), "TestSplashAbilitySet" );
		}
	}
}
