/**
See licence file in root folder, MIT.txt
*/
#include "EnemyTest.hpp"

#include <Game.hpp>

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
			Game l_game;
			float l_speed{ 100.0f };
			uint32_t l_life{ 1000u };
			auto l_enemy = l_game.CreateEnemy( l_speed, l_life );
			CT_EQUAL( l_game.GetComponentData< EnemyState >( l_enemy, l_game.GetComponent( Game::StateComponent ) ).GetValue(), EnemyState::eWalking );
			CT_EQUAL( l_game.GetComponentData< float >( l_enemy, l_game.GetComponent( Game::SpeedComponent ) ).GetValue(), l_speed );
			CT_EQUAL( l_game.GetComponentData< uint32_t >( l_enemy, l_game.GetComponent( Game::LifeComponent ) ).GetValue(), l_life );
		}
	}
}
