/**
See licence file in root folder, MIT.txt
*/
#include "EnemyTest.hpp"

#include <Ecs.hpp>
#include <EnemyState.hpp>

namespace
{
	std::ostream & operator<<( std::ostream & p_stream, orastus::EnemyState p_value )
	{
		p_stream << orastus::GetName( p_value );
		return p_stream;
	}
}

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

		void EnemyTest::DoRegisterTests()
		{
			DoRegisterTest( "EnemyTest::Creation", std::bind( &EnemyTest::Creation, this ) );
		}

		void EnemyTest::Creation()
		{
			Ecs l_ecs;
			float l_speed{ 100.0f };
			uint32_t l_life{ 1000u };
			auto l_enemy = l_ecs.CreateEnemy( l_speed, l_life );
			CT_EQUAL( l_ecs.GetComponentData< EnemyState >( l_enemy, l_ecs.GetComponent( Ecs::StateComponent ) ).GetValue(), EnemyState::eWalking );
			CT_EQUAL( l_ecs.GetComponentData< float >( l_enemy, l_ecs.GetComponent( Ecs::SpeedComponent ) ).GetValue(), l_speed );
			CT_EQUAL( l_ecs.GetComponentData< uint32_t >( l_enemy, l_ecs.GetComponent( Ecs::LifeComponent ) ).GetValue(), l_life );
		}
	}
}
