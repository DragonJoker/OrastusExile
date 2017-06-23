/**
See licence file in root folder, MIT.txt
*/
#include "BulletTest.hpp"

#include <ECS/Ecs.hpp>
#include <ECS/TrackData.hpp>

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
			Ecs l_ecs;
			float l_speed{ 100.0f };
			uint32_t l_damage{ 1000u };
			auto l_bullet = l_ecs.CreateBullet( nullptr
				, std::make_shared< TrackData >( Entity{}, l_speed, l_damage ) );
			CT_EQUAL( l_ecs.GetComponentData< TrackDataPtr >( l_bullet, l_ecs.GetComponent( Ecs::TrackComponent ) ).GetValue()->m_speed, l_speed );
			CT_EQUAL( l_ecs.GetComponentData< TrackDataPtr >( l_bullet, l_ecs.GetComponent( Ecs::TrackComponent ) ).GetValue()->m_damage, l_damage );
		}
	}
}
