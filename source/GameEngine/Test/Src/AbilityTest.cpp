/**
See licence file in root folder, MIT.txt
*/
#include "AbilityTest.hpp"

#include <Ability.hpp>

namespace
{
	std::ostream & operator<<( std::ostream & p_stream, orastus::Milliseconds const & p_value )
	{
		p_stream << p_value.count() << " ms";
		return p_stream;
	}

	template< typename T >
	std::ostream & operator<<( std::ostream & p_stream, orastus::Ability< T > const & p_value )
	{
		p_stream << p_value.GetValue();
		return p_stream;
	}
}

namespace orastus
{
	namespace test
	{
		AbilityTest::AbilityTest()
			: Testing::TestCase{ "AbilityTest" }
		{
		}

		AbilityTest::~AbilityTest()
		{
		}

		void AbilityTest::DoRegisterTests()
		{
			DoRegisterTest( "AbilityCreation", std::bind( &AbilityTest::AbilityCreation, this ) );
			DoRegisterTest( "AbilityComparison", std::bind( &AbilityTest::AbilityComparison, this ) );
		}

		void AbilityTest::AbilityCreation()
		{
			Milliseconds l_time{ 100 };
			CooldownAbility l_cooldown{ l_time };
			CT_EQUAL( l_cooldown.GetValue(), l_time );

			uint32_t l_dmg{ 1000 };
			DamageAbility l_damage{ l_dmg };
			CT_EQUAL( l_damage.GetValue(), l_dmg );

			float l_rad{ 0.56f };
			RangeAbility l_range{ l_rad };
			CT_EQUAL( l_range.GetValue(), l_rad );
		}

		void AbilityTest::AbilityComparison()
		{
			Milliseconds l_time1{ 100 };
			Milliseconds l_time2{ 101 };
			CooldownAbility l_cooldown1{ l_time1 };
			CooldownAbility l_cooldown2{ l_time2 };
			CT_NEQUAL( l_cooldown1, l_cooldown2 );
			CT_EQUAL( l_cooldown1, l_cooldown1 );

			uint32_t l_dmg1{ 100 };
			uint32_t l_dmg2{ 101 };
			DamageAbility l_damage1{ l_dmg1 };
			DamageAbility l_damage2{ l_dmg2 };
			CT_NEQUAL( l_damage1, l_damage2 );
			CT_EQUAL( l_damage1, l_damage1 );

			float l_rad1{ 0.5f };
			float l_rad2{ 0.6f };
			RangeAbility l_range1{ l_rad1 };
			RangeAbility l_range2{ l_rad2 };
			CT_NEQUAL( l_range1, l_range2 );
			CT_EQUAL( l_range1, l_range1 );

			DamageAbility l_damage{ 10 };
			RangeAbility l_range{ 10 };
			CT_NEQUAL( l_range, l_damage );
		}
	}
}
