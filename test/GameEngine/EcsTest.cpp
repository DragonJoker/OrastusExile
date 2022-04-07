/**
See licence file in root folder, MIT.txt
*/
#include "EcsTest.hpp"

#include <GameEngine/ECS/Ecs.hpp>

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

		void EcsTest::doRegisterTests()
		{
			doRegisterTest( "EcsTest::Hashing", std::bind( &EcsTest::Hashing, this ) );
		}

		void EcsTest::Hashing()
		{
			auto l_id = Ecs::hash( "ABCDEFGH" );
			auto l_un = Ecs::unhash( l_id );
			CT_EQUAL( l_un, String( "ABCDEFGH" ) );
		}
	}
}
