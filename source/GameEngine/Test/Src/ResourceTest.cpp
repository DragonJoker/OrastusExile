/**
See licence file in root folder, MIT.txt
*/
#include "ResourceTest.hpp"

#include <Resource.hpp>

namespace
{
	std::ostream & operator<<( std::ostream & p_stream, orastus::Resource::Kind p_kind )
	{
		p_stream << orastus::Resource::GetKindName( p_kind );
		return p_stream;
	}
}

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

		void ResourceTest::DoRegisterTests()
		{
			DoRegisterTest( "ResourceCreation", std::bind( &ResourceTest::ResourceCreation, this ) );
			DoRegisterTest( "ResourceAddition", std::bind( &ResourceTest::ResourceAddition, this ) );
			DoRegisterTest( "ResourceSubtraction", std::bind( &ResourceTest::ResourceSubtraction, this ) );
		}

		void ResourceTest::ResourceCreation()
		{
			Resource l_cloth{ Resource::Kind::eCloth, 10u };
			CT_EQUAL( l_cloth.GetKind(), Resource::Kind::eCloth );
			CT_EQUAL( l_cloth.GetCount(), 10u );

			Resource l_leather{ Resource::Kind::eLeather, 10u };
			CT_EQUAL( l_leather.GetKind(), Resource::Kind::eLeather );
			CT_EQUAL( l_leather.GetCount(), 10u );

			Resource l_wood{ Resource::Kind::eWood, 10u };
			CT_EQUAL( l_wood.GetKind(), Resource::Kind::eWood );
			CT_EQUAL( l_wood.GetCount(), 10u );

			Resource l_iron{ Resource::Kind::eIron, 10u };
			CT_EQUAL( l_iron.GetKind(), Resource::Kind::eIron );
			CT_EQUAL( l_iron.GetCount(), 10u );

			Resource l_gold{ Resource::Kind::eGold, 10u };
			CT_EQUAL( l_gold.GetKind(), Resource::Kind::eGold );
			CT_EQUAL( l_gold.GetCount(), 10u );
		}

		void ResourceTest::ResourceAddition()
		{
			Resource l_cloth{ Resource::Kind::eCloth, 10u };
			CT_EQUAL( l_cloth.GetCount(), 10u );

			l_cloth += 5u;
			CT_EQUAL( l_cloth.GetCount(), 15u );

			Resource l_cloth2{ Resource::Kind::eCloth, 10u };
			l_cloth += l_cloth2;
			CT_EQUAL( l_cloth.GetCount(), 25u );

			Resource l_leather{ Resource::Kind::eLeather, 10u };
			CT_CHECK_THROW( l_cloth += l_leather );
		}

		void ResourceTest::ResourceSubtraction()
		{
			Resource l_cloth{ Resource::Kind::eCloth, 10u };
			CT_EQUAL( l_cloth.GetCount(), 10u );

			l_cloth -= 5u;
			CT_EQUAL( l_cloth.GetCount(), 5u );

			Resource l_cloth2{ Resource::Kind::eCloth, 10u };
			l_cloth -= l_cloth2;
			CT_EQUAL( l_cloth.GetCount(), 0u );

			Resource l_leather{ Resource::Kind::eLeather, 10u };
			CT_CHECK_THROW( l_cloth -= l_leather );
		}
	}
}
