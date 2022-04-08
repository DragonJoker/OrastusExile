/**
See licence file in root folder, MIT.txt
*/
#include "ComponentTest.hpp"

#include <GameEngine/ECS/Component.hpp>
#include <GameEngine/ECS/Ecs.hpp>

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
			auto name{ Ecs::hash( "aaaaaaaa" ) };
			String desc{ cuT( "A" ) };
			Component comp{ name, desc };
			CT_EQUAL( comp.getName(), name );
			CT_EQUAL( comp.getDesc(), desc );
		}

		void ComponentTest::Comparison()
		{
			Component comp1{ Ecs::hash( "aaaaaaaa" ), cuT( "A" ) };
			Component comp2{ Ecs::hash( "bbbbbbbb" ), cuT( "B" ) };
			CT_NEQUAL( comp1.getId(), comp2.getId() );
			CT_NEQUAL( comp1.getName(), comp2.getName() );
			CT_NEQUAL( comp1.getDesc(), comp2.getDesc() );
		}
	}
}
