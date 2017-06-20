/**
See licence file in root folder, MIT.txt
*/
#include "EntityTest.hpp"

#include <Entity.hpp>

namespace orastus
{
	namespace test
	{
		EntityTest::EntityTest()
			: Testing::TestCase{ "EntityTest" }
		{
		}

		EntityTest::~EntityTest()
		{
		}

		void EntityTest::DoRegisterTests()
		{
			DoRegisterTest( "EntityTest::Creation", std::bind( &EntityTest::Creation, this ) );
		}

		void EntityTest::Creation()
		{
			Entity l_1{ cuT( "1" ) };
			Entity l_2{ cuT( "2" ) };
			CT_NEQUAL( l_1.GetId(), l_2.GetId() );
		}
	}
}
