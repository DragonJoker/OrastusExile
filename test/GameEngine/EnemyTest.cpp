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
			Ecs l_ecs;
			float l_speed{ 100.0f };
			uint32_t l_life{ 1000u };
			auto l_enemy = l_ecs.createEnemy( l_speed, l_life, nullptr, nullptr );
			CT_EQUAL( l_ecs.getComponentData< float >( l_enemy, l_ecs.getComponent( Ecs::SpeedComponent ) ).getValue(), l_speed );
			CT_EQUAL( l_ecs.getComponentData< uint32_t >( l_enemy, l_ecs.getComponent( Ecs::LifeComponent ) ).getValue(), l_life );
		}
	}
}
