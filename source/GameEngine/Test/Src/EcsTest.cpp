/**
See licence file in root folder, MIT.txt
*/
#include "EcsTest.hpp"

#include <ECS/Ecs.hpp>

namespace orastus
{
	namespace test
	{
		EcsTest::EcsTest()
			: Testing::TestCase{ "EcsTest" }
		{
		}

		EcsTest::~EcsTest()
		{
		}

		void EcsTest::DoRegisterTests()
		{
			DoRegisterTest( "EcsTest::Hashing", std::bind( &EcsTest::Hashing, this ) );
		}

		void EcsTest::Hashing()
		{
			auto l_id = Ecs::Hash( "ABCDEFGH" );
			auto l_un = Ecs::Unhash( l_id );
			CT_EQUAL( l_un, String( "ABCDEFGH" ) );
		}
	}
}
