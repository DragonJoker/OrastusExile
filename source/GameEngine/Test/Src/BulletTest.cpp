/**
See licence file in root folder, MIT.txt
*/
#include "BulletTest.hpp"

#include <Game.hpp>

namespace orastus
{
	namespace test
	{
		BulletTest::BulletTest()
			: Testing::TestCase{ "BulletTest" }
		{
		}

		BulletTest::~BulletTest()
		{
		}

		void BulletTest::DoRegisterTests()
		{
			DoRegisterTest( "BulletTest::Creation", std::bind( &BulletTest::Creation, this ) );
		}

		void BulletTest::Creation()
		{
			Game l_game;
			float l_speed{ 100.0f };
			uint32_t l_damage{ 1000u };
			auto l_bullet = l_game.CreateBullet( l_speed, l_damage );
			CT_EQUAL( l_game.GetComponentData< float >( l_bullet, l_game.GetComponent( Game::SpeedComponent ) ).GetValue(), l_speed );
			CT_EQUAL( l_game.GetComponentData< uint32_t >( l_bullet, l_game.GetComponent( Game::DamageComponent ) ).GetValue(), l_damage );
		}
	}
}
