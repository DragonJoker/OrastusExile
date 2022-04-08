/**
See licence file in root folder, MIT.txt
*/
#include "ComponentDataTest.hpp"

#include <GameEngine/ECS/ComponentData.hpp>

namespace orastus
{
	namespace test
	{
		ComponentDataTest::ComponentDataTest()
			: Testing::TestCase{ "ComponentDataTest" }
		{
		}

		ComponentDataTest::~ComponentDataTest()
		{
		}

		void ComponentDataTest::doRegisterTests()
		{
			doRegisterTest( "ComponentDataTest::Creation", std::bind( &ComponentDataTest::Creation, this ) );
			doRegisterTest( "ComponentDataTest::Comparison", std::bind( &ComponentDataTest::Comparison, this ) );
		}

		void ComponentDataTest::Creation()
		{
			Milliseconds time{ 100 };
			ComponentData< Milliseconds > cooldown{ time };
			CT_EQUAL( cooldown.getValue(), time );

			uint32_t dmg{ 1000 };
			ComponentData< uint32_t > damage{ dmg };
			CT_EQUAL( damage.getValue(), dmg );

			float rng{ 0.56f };
			ComponentData< float > range{ rng };
			CT_EQUAL( range.getValue(), rng );
		}

		void ComponentDataTest::Comparison()
		{
			Milliseconds time1{ 100 };
			Milliseconds time2{ 101 };
			ComponentData< Milliseconds > cooldown1{ time1 };
			ComponentData< Milliseconds > cooldown2{ time2 };
			CT_NEQUAL( cooldown1, cooldown2 );
			CT_EQUAL( cooldown1, cooldown1 );

			uint32_t dmg1{ 100 };
			uint32_t dmg2{ 101 };
			ComponentData< uint32_t > damage1{ dmg1 };
			ComponentData< uint32_t > damage2{ dmg2 };
			CT_NEQUAL( damage1, damage2 );
			CT_EQUAL( damage1, damage1 );

			float rg1{ 0.5f };
			float rg2{ 0.6f };
			ComponentData< float > range1{ rg1 };
			ComponentData< float > range2{ rg2 };
			CT_NEQUAL( range1, range2 );
			CT_EQUAL( range1, range1 );

			ComponentData< float > range{ rg1 };
			ComponentData< uint32_t > damage{ dmg2 };
			CT_NEQUAL( range, damage );
		}
	}
}
