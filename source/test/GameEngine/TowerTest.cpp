/**
See licence file in root folder, MIT.txt
*/
#include "TowerTest.hpp"

#include <ECS/Ecs.hpp>
#include <State/TowerState.hpp>

namespace orastus
{
	namespace test
	{
		TowerTest::TowerTest()
			: Testing::TestCase{ "TowerTest" }
		{
		}

		TowerTest::~TowerTest()
		{
		}

		void TowerTest::doRegisterTests()
		{
			doRegisterTest( "TowerTest::Creation", std::bind( &TowerTest::Creation, this ) );
		}

		void TowerTest::Creation()
		{
			Ecs l_ecs;
			Milliseconds l_cooldown{ 100_ms };
			uint32_t l_damage{ 1000u };
			float l_range{ 0.56f };
			float l_speed{ 2.14f };
			uint32_t l_level{ 1u };
			auto l_tower = l_ecs.createTower( l_cooldown, l_damage, l_range, l_speed, l_level, nullptr, nullptr, nullptr );
			CT_EQUAL( l_ecs.getComponentData< Milliseconds >( l_tower, l_ecs.getComponent( Ecs::CooldownComponent ) ).getValue(), l_cooldown );
			CT_EQUAL( l_ecs.getComponentData< uint32_t >( l_tower, l_ecs.getComponent( Ecs::DamageComponent ) ).getValue(), l_damage );
			CT_EQUAL( l_ecs.getComponentData< float >( l_tower, l_ecs.getComponent( Ecs::RangeComponent ) ).getValue(), l_range );
			CT_EQUAL( l_ecs.getComponentData< float >( l_tower, l_ecs.getComponent( Ecs::SpeedComponent ) ).getValue(), l_speed );
			CT_EQUAL( l_ecs.getComponentData< uint32_t >( l_tower, l_ecs.getComponent( Ecs::LevelComponent ) ).getValue(), l_level );

			uint32_t l_splashDamage{ 500u };
			float l_splashRange{ 0.1f };
			auto l_splashTower = l_ecs.createTower( l_cooldown, l_damage, l_range, l_speed, l_splashDamage, l_splashRange, l_level, nullptr, nullptr, nullptr );
			CT_EQUAL( l_ecs.getComponentData< Milliseconds >( l_splashTower, l_ecs.getComponent( Ecs::CooldownComponent ) ).getValue(), l_cooldown );
			CT_EQUAL( l_ecs.getComponentData< uint32_t >( l_splashTower, l_ecs.getComponent( Ecs::DamageComponent ) ).getValue(), l_damage );
			CT_EQUAL( l_ecs.getComponentData< float >( l_splashTower, l_ecs.getComponent( Ecs::RangeComponent ) ).getValue(), l_range );
			CT_EQUAL( l_ecs.getComponentData< uint32_t >( l_splashTower, l_ecs.getComponent( Ecs::SplashDamageComponent ) ).getValue(), l_splashDamage );
			CT_EQUAL( l_ecs.getComponentData< float >( l_splashTower, l_ecs.getComponent( Ecs::SplashRangeComponent ) ).getValue(), l_splashRange );
			CT_EQUAL( l_ecs.getComponentData< float >( l_splashTower, l_ecs.getComponent( Ecs::SpeedComponent ) ).getValue(), l_speed );
			CT_EQUAL( l_ecs.getComponentData< uint32_t >( l_splashTower, l_ecs.getComponent( Ecs::LevelComponent ) ).getValue(), l_level );
		}
	}
}
