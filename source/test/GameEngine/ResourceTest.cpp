/**
See licence file in root folder, MIT.txt
*/
#include "ResourceTest.hpp"

#include <Resource.hpp>

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
			Resource l_cloth{ Resource::Kind::eCloth, 10u };
			CT_EQUAL( l_cloth.getKind(), Resource::Kind::eCloth );
			CT_EQUAL( l_cloth.getCount(), 10u );

			Resource l_leather{ Resource::Kind::eLeather, 10u };
			CT_EQUAL( l_leather.getKind(), Resource::Kind::eLeather );
			CT_EQUAL( l_leather.getCount(), 10u );

			Resource l_wood{ Resource::Kind::eWood, 10u };
			CT_EQUAL( l_wood.getKind(), Resource::Kind::eWood );
			CT_EQUAL( l_wood.getCount(), 10u );

			Resource l_iron{ Resource::Kind::eIron, 10u };
			CT_EQUAL( l_iron.getKind(), Resource::Kind::eIron );
			CT_EQUAL( l_iron.getCount(), 10u );

			Resource l_gold{ Resource::Kind::eGold, 10u };
			CT_EQUAL( l_gold.getKind(), Resource::Kind::eGold );
			CT_EQUAL( l_gold.getCount(), 10u );
		}

		void ResourceTest::ResourceAddition()
		{
			Resource l_cloth{ Resource::Kind::eCloth, 10u };
			CT_EQUAL( l_cloth.getCount(), 10u );

			l_cloth += 5u;
			CT_EQUAL( l_cloth.getCount(), 15u );

			Resource l_cloth2{ Resource::Kind::eCloth, 10u };
			l_cloth += l_cloth2;
			CT_EQUAL( l_cloth.getCount(), 25u );

			Resource l_leather{ Resource::Kind::eLeather, 10u };
			CT_CHECK_THROW( l_cloth += l_leather );
		}

		void ResourceTest::ResourceSubtraction()
		{
			Resource l_cloth{ Resource::Kind::eCloth, 10u };
			CT_EQUAL( l_cloth.getCount(), 10u );

			l_cloth -= 5u;
			CT_EQUAL( l_cloth.getCount(), 5u );

			Resource l_cloth2{ Resource::Kind::eCloth, 10u };
			l_cloth -= l_cloth2;
			CT_EQUAL( l_cloth.getCount(), 0u );

			Resource l_leather{ Resource::Kind::eLeather, 10u };
			CT_CHECK_THROW( l_cloth -= l_leather );
		}
	}
}
