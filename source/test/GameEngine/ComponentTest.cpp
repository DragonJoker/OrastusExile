/**
See licence file in root folder, MIT.txt
*/
#include "ComponentTest.hpp"

#include <ECS/Component.hpp>
#include <ECS/Ecs.hpp>

namespace orastus
{
	namespace test
	{
		ComponentTest::ComponentTest()
			: Testing::TestCase{ "ComponentTest" }
		{
		}

		ComponentTest::~ComponentTest()
		{
		}

		void ComponentTest::doRegisterTests()
		{
			doRegisterTest( "ComponentTest::Creation", std::bind( &ComponentTest::Creation, this ) );
			doRegisterTest( "ComponentTest::Comparison", std::bind( &ComponentTest::Comparison, this ) );
		}

		void ComponentTest::Creation()
		{
			auto l_name{ Ecs::hash( "aaaaaaaa" ) };
			String l_desc{ cuT( "A" ) };
			Component l_comp{ l_name, l_desc };
			CT_EQUAL( l_comp.getName(), l_name );
			CT_EQUAL( l_comp.getDesc(), l_desc );
		}

		void ComponentTest::Comparison()
		{
			Component l_comp1{ Ecs::hash( "aaaaaaaa" ), cuT( "A" ) };
			Component l_comp2{ Ecs::hash( "bbbbbbbb" ), cuT( "B" ) };
			CT_NEQUAL( l_comp1.getId(), l_comp2.getId() );
			CT_NEQUAL( l_comp1.getName(), l_comp2.getName() );
			CT_NEQUAL( l_comp1.getDesc(), l_comp2.getDesc() );
		}
	}
}
