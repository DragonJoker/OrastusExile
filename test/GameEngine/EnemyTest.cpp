/**
See licence file in root folder, MIT.txt
*/
#include "EnemyTest.hpp"

#include <GameEngine/ECS/Ecs.hpp>
#include <GameEngine/ECS/Enemy.hpp>

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
			auto enemy = ecs.createEnemy( speed, life, nullptr, nullptr, {} );
			auto & data = ecs.getComponentData< EnemyData >( enemy
				, ecs.getComponent( Ecs::EnemyStateComponent ) ).getValue();
			CT_EQUAL( data.speed, speed );
			CT_EQUAL( data.life, life );
		}
	}
}
