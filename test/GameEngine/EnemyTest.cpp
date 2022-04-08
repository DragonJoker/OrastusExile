/**
See licence file in root folder, MIT.txt
*/
#include "EnemyTest.hpp"

#include <GameEngine/ECS/Ecs.hpp>
#include <GameEngine/State/EnemyState.hpp>

namespace orastus
{
	namespace test
	{
		EnemyTest::EnemyTest()
			: Testing::TestCase{ "EnemyTest" }
		{
		}

		EnemyTest::~EnemyTest()
		{
		}

		void EnemyTest::doRegisterTests()
		{
			doRegisterTest( "EnemyTest::Creation", std::bind( &EnemyTest::Creation, this ) );
		}

		void EnemyTest::Creation()
		{
			Ecs ecs;
			float speed{ 100.0f };
			uint32_t life{ 1000u };
			auto enemy = ecs.createEnemy( speed, life, nullptr, nullptr );
			CT_EQUAL( ecs.getComponentData< float >( enemy, ecs.getComponent( Ecs::SpeedComponent ) ).getValue(), speed );
			CT_EQUAL( ecs.getComponentData< uint32_t >( enemy, ecs.getComponent( Ecs::LifeComponent ) ).getValue(), life );
		}
	}
}
