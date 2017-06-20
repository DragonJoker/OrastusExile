/**
See licence file in root folder, MIT.txt
*/
#include "ComponentTest.hpp"

#include <Component.hpp>

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

		void ComponentTest::DoRegisterTests()
		{
			DoRegisterTest( "ComponentTest::Creation", std::bind( &ComponentTest::Creation, this ) );
			DoRegisterTest( "ComponentTest::Comparison", std::bind( &ComponentTest::Comparison, this ) );
		}

		void ComponentTest::Creation()
		{
			String l_name{ cuT( "a" ) };
			String l_desc{ cuT( "A" ) };
			Component l_comp{ l_name, l_desc };
			CT_EQUAL( l_comp.GetName(), l_name );
			CT_EQUAL( l_comp.GetDesc(), l_desc );
		}

		void ComponentTest::Comparison()
		{
			Component l_comp1{ cuT( "a" ), cuT( "A" ) };
			Component l_comp2{ cuT( "b" ), cuT( "B" ) };
			CT_NEQUAL( l_comp1.GetId(), l_comp2.GetId() );
			CT_NEQUAL( l_comp1.GetName(), l_comp2.GetName() );
			CT_NEQUAL( l_comp1.GetDesc(), l_comp2.GetDesc() );
		}
	}
}
