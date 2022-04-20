/**
See licence file in root folder, MIT.txt
*/
#include "BulletTest.hpp"

#include <GameEngine/ECS/Bullet.hpp>
#include <GameEngine/ECS/Ecs.hpp>
#include <GameEngine/ECS/SoundSource.hpp>

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

		void BulletTest::doRegisterTests()
		{
			doRegisterTest( "BulletTest::Creation", std::bind( &BulletTest::Creation, this ) );
		}

		void BulletTest::Creation()
		{
			Ecs ecs;
			float speed{ 100.0f };
			uint32_t damage{ 1000u };
			auto bullet = ecs.createBullet( nullptr
				, {}
				, std::make_unique< TrackData >( Entity{}, speed, damage ) );
			auto & data = ecs.getComponentData< BulletData >( bullet
				, ecs.getComponent( Ecs::BulletStateComponent ) ).getValue();
			CT_EQUAL( data.status, BulletStatus::eFlying );
			CT_EQUAL( data.track->speed, speed );
			CT_EQUAL( data.track->damage, damage );
		}
	}
}
