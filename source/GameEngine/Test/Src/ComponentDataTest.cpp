/**
See licence file in root folder, MIT.txt
*/
#include "ComponentDataTest.hpp"

#include <ECS/ComponentData.hpp>

namespace
{
	std::ostream & operator<<( std::ostream & p_stream, orastus::Milliseconds const & p_value )
	{
		p_stream << p_value.count() << " ms";
		return p_stream;
	}

	template< typename T >
	std::ostream & operator<<( std::ostream & p_stream, orastus::ComponentData< T > const & p_value )
	{
		p_stream << p_value.GetValue();
		return p_stream;
	}
}

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

		void ComponentDataTest::DoRegisterTests()
		{
			DoRegisterTest( "ComponentDataTest::Creation", std::bind( &ComponentDataTest::Creation, this ) );
			DoRegisterTest( "ComponentDataTest::Comparison", std::bind( &ComponentDataTest::Comparison, this ) );
		}

		void ComponentDataTest::Creation()
		{
			Milliseconds l_time{ 100 };
			ComponentData< Milliseconds > l_cooldown{ l_time };
			CT_EQUAL( l_cooldown.GetValue(), l_time );

			uint32_t l_dmg{ 1000 };
			ComponentData< uint32_t > l_damage{ l_dmg };
			CT_EQUAL( l_damage.GetValue(), l_dmg );

			float l_rng{ 0.56f };
			ComponentData< float > l_range{ l_rng };
			CT_EQUAL( l_range.GetValue(), l_rng );
		}

		void ComponentDataTest::Comparison()
		{
			Milliseconds l_time1{ 100 };
			Milliseconds l_time2{ 101 };
			ComponentData< Milliseconds > l_cooldown1{ l_time1 };
			ComponentData< Milliseconds > l_cooldown2{ l_time2 };
			CT_NEQUAL( l_cooldown1, l_cooldown2 );
			CT_EQUAL( l_cooldown1, l_cooldown1 );

			uint32_t l_dmg1{ 100 };
			uint32_t l_dmg2{ 101 };
			ComponentData< uint32_t > l_damage1{ l_dmg1 };
			ComponentData< uint32_t > l_damage2{ l_dmg2 };
			CT_NEQUAL( l_damage1, l_damage2 );
			CT_EQUAL( l_damage1, l_damage1 );

			float l_rad1{ 0.5f };
			float l_rad2{ 0.6f };
			ComponentData< float > l_range1{ l_rad1 };
			ComponentData< float > l_range2{ l_rad2 };
			CT_NEQUAL( l_range1, l_range2 );
			CT_EQUAL( l_range1, l_range1 );

			ComponentData< float > l_range{ l_rad1 };
			ComponentData< uint32_t > l_damage{ l_dmg2 };
			CT_NEQUAL( l_range, l_damage );
		}
	}
}
