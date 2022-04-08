/**
See licence file in root folder, MIT.txt
*/
#include "ResourceTest.hpp"

#include <GameEngine/Resource.hpp>

namespace orastus
{
	namespace test
	{
		ResourceTest::ResourceTest()
			: Testing::TestCase{ "ResourceTest" }
		{
		}

		ResourceTest::~ResourceTest()
		{
		}

		void ResourceTest::doRegisterTests()
		{
			doRegisterTest( "ResourceCreation", std::bind( &ResourceTest::ResourceCreation, this ) );
			doRegisterTest( "ResourceAddition", std::bind( &ResourceTest::ResourceAddition, this ) );
			doRegisterTest( "ResourceSubtraction", std::bind( &ResourceTest::ResourceSubtraction, this ) );
		}

		void ResourceTest::ResourceCreation()
		{
			Resource cloth{ Resource::Kind::eCloth, 10u };
			CT_EQUAL( cloth.getKind(), Resource::Kind::eCloth );
			CT_EQUAL( cloth.getCount(), 10u );

			Resource leather{ Resource::Kind::eLeather, 10u };
			CT_EQUAL( leather.getKind(), Resource::Kind::eLeather );
			CT_EQUAL( leather.getCount(), 10u );

			Resource wood{ Resource::Kind::eWood, 10u };
			CT_EQUAL( wood.getKind(), Resource::Kind::eWood );
			CT_EQUAL( wood.getCount(), 10u );

			Resource iron{ Resource::Kind::eIron, 10u };
			CT_EQUAL( iron.getKind(), Resource::Kind::eIron );
			CT_EQUAL( iron.getCount(), 10u );

			Resource gold{ Resource::Kind::eGold, 10u };
			CT_EQUAL( gold.getKind(), Resource::Kind::eGold );
			CT_EQUAL( gold.getCount(), 10u );
		}

		void ResourceTest::ResourceAddition()
		{
			Resource cloth{ Resource::Kind::eCloth, 10u };
			CT_EQUAL( cloth.getCount(), 10u );

			cloth += 5u;
			CT_EQUAL( cloth.getCount(), 15u );

			Resource cloth2{ Resource::Kind::eCloth, 10u };
			cloth += cloth2;
			CT_EQUAL( cloth.getCount(), 25u );

			Resource leather{ Resource::Kind::eLeather, 10u };
			CT_CHECK_THROW( cloth += leather );
		}

		void ResourceTest::ResourceSubtraction()
		{
			Resource cloth{ Resource::Kind::eCloth, 10u };
			CT_EQUAL( cloth.getCount(), 10u );

			cloth -= 5u;
			CT_EQUAL( cloth.getCount(), 5u );

			Resource cloth2{ Resource::Kind::eCloth, 10u };
			cloth -= cloth2;
			CT_EQUAL( cloth.getCount(), 0u );

			Resource leather{ Resource::Kind::eLeather, 10u };
			CT_CHECK_THROW( cloth -= leather );
		}
	}
}
