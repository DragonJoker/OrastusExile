/**
See licence file in root folder, MIT.txt
*/
#include "EntityTest.hpp"

#include <GameEngine/ECS/Entity.hpp>

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

		void EntityTest::doRegisterTests()
		{
			doRegisterTest( "EntityTest::Creation", std::bind( &EntityTest::Creation, this ) );
		}

		void EntityTest::Creation()
		{
#if defined( NDEBUG )
			CT_EQUAL( sizeof( Entity ), sizeof( Id ) );
#else
			CT_EQUAL( sizeof( Entity ), sizeof( Id ) + sizeof( String ) );
#endif
			Entity i1{ cuT( "1" ) };
			Entity i2{ cuT( "2" ) };
			CT_NEQUAL( i1.getId(), i2.getId() );
		}
	}
}
