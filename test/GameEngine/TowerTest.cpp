/**
See licence file in root folder, MIT.txt
*/
#include "TowerTest.hpp"

#include <GameEngine/ECS/Ecs.hpp>
#include <GameEngine/ECS/AnimationData.hpp>
#include <GameEngine/ECS/AttackData.hpp>
#include <GameEngine/ECS/SoundSource.hpp>
#include <GameEngine/State/TowerState.hpp>

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
			Ecs ecs;
			Milliseconds cooldown{ 100_ms };
			uint32_t damage{ 1000u };
			float range{ 0.56f };
			float speed{ 2.14f };
			uint32_t level{ 1u };
			auto tower = ecs.createTower( cooldown, damage, range, speed, level, nullptr, nullptr, nullptr, {}, nullptr );
			CT_EQUAL( ecs.getComponentData< Milliseconds >( tower, ecs.getComponent( Ecs::CooldownComponent ) ).getValue(), cooldown );
			CT_EQUAL( ecs.getComponentData< Milliseconds >( tower, ecs.getComponent( Ecs::TimeoutComponent ) ).getValue(), 0_ms );
			CT_EQUAL( ecs.getComponentData< uint32_t >( tower, ecs.getComponent( Ecs::DamageComponent ) ).getValue(), damage );
			CT_EQUAL( ecs.getComponentData< float >( tower, ecs.getComponent( Ecs::RangeComponent ) ).getValue(), range );
			CT_EQUAL( ecs.getComponentData< float >( tower, ecs.getComponent( Ecs::SpeedComponent ) ).getValue(), speed );
			CT_EQUAL( ecs.getComponentData< uint32_t >( tower, ecs.getComponent( Ecs::LevelComponent ) ).getValue(), level );

			uint32_t splashDamage{ 500u };
			float splashRange{ 0.1f };
			auto splashTower = ecs.createTower( cooldown, damage, range, speed, splashDamage, splashRange, level, nullptr, nullptr, nullptr, {}, nullptr );
			CT_EQUAL( ecs.getComponentData< Milliseconds >( splashTower, ecs.getComponent( Ecs::CooldownComponent ) ).getValue(), cooldown );
			CT_EQUAL( ecs.getComponentData< Milliseconds >( splashTower, ecs.getComponent( Ecs::TimeoutComponent ) ).getValue(), 0_ms );
			CT_EQUAL( ecs.getComponentData< uint32_t >( splashTower, ecs.getComponent( Ecs::DamageComponent ) ).getValue(), damage );
			CT_EQUAL( ecs.getComponentData< float >( splashTower, ecs.getComponent( Ecs::RangeComponent ) ).getValue(), range );
			CT_EQUAL( ecs.getComponentData< uint32_t >( splashTower, ecs.getComponent( Ecs::SplashDamageComponent ) ).getValue(), splashDamage );
			CT_EQUAL( ecs.getComponentData< float >( splashTower, ecs.getComponent( Ecs::SplashRangeComponent ) ).getValue(), splashRange );
			CT_EQUAL( ecs.getComponentData< float >( splashTower, ecs.getComponent( Ecs::SpeedComponent ) ).getValue(), speed );
			CT_EQUAL( ecs.getComponentData< uint32_t >( splashTower, ecs.getComponent( Ecs::LevelComponent ) ).getValue(), level );
		}
	}
}
