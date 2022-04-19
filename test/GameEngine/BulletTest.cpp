/**
See licence file in root folder, MIT.txt
*/
#include "BulletTest.hpp"

#include <GameEngine/ECS/Ecs.hpp>
#include <GameEngine/ECS/SoundSource.hpp>
#include <GameEngine/ECS/TrackData.hpp>

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
			CT_EQUAL( ecs.getComponentData< TrackDataPtr >( bullet, ecs.getComponent( Ecs::TrackComponent ) ).getValue()->speed, speed );
			CT_EQUAL( ecs.getComponentData< TrackDataPtr >( bullet, ecs.getComponent( Ecs::TrackComponent ) ).getValue()->damage, damage );
		}
	}
}
