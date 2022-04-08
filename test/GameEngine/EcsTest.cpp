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
			auto id = Ecs::hash( "ABCDEFGH" );
			auto un = Ecs::unhash( id );
			CT_EQUAL( un, String( "ABCDEFGH" ) );
		}
	}
}
