/**
See licence file in root folder, MIT.txt
*/
#include "TowerTest.hpp"

#include <Game.hpp>
#include <TowerState.hpp>

namespace
{
	std::ostream & operator<<( std::ostream & p_stream, orastus::TowerState p_value )
	{
		p_stream << orastus::GetName( p_value );
		return p_stream;
	}
}

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

		void TowerTest::DoRegisterTests()
		{
			DoRegisterTest( "TowerTest::Creation", std::bind( &TowerTest::Creation, this ) );
		}

		void TowerTest::Creation()
		{
			Ecs l_ecs;
			Milliseconds l_cooldown{ 100_ms };
			uint32_t l_damage{ 1000u };
			float l_range{ 0.56f };
			float l_speed{ 2.14f };
			uint32_t l_level{ 1u };
			auto l_tower = l_ecs.CreateTower( l_cooldown, l_damage, l_range, l_speed, l_level );
			CT_EQUAL( l_ecs.GetComponentData< TowerState >( l_tower, l_ecs.GetComponent( Ecs::StateComponent ) ).GetValue(), TowerState::eIdle );
			CT_EQUAL( l_ecs.GetComponentData< Milliseconds >( l_tower, l_ecs.GetComponent( Ecs::CooldownComponent ) ).GetValue(), l_cooldown );
			CT_EQUAL( l_ecs.GetComponentData< uint32_t >( l_tower, l_ecs.GetComponent( Ecs::DamageComponent ) ).GetValue(), l_damage );
			CT_EQUAL( l_ecs.GetComponentData< float >( l_tower, l_ecs.GetComponent( Ecs::RangeComponent ) ).GetValue(), l_range );
			CT_EQUAL( l_ecs.GetComponentData< float >( l_tower, l_ecs.GetComponent( Ecs::SpeedComponent ) ).GetValue(), l_speed );
			CT_EQUAL( l_ecs.GetComponentData< uint32_t >( l_tower, l_ecs.GetComponent( Ecs::LevelComponent ) ).GetValue(), l_level );

			uint32_t l_splashDamage{ 500u };
			float l_splashRange{ 0.1f };
			auto l_splashTower = l_ecs.CreateTower( l_cooldown, l_damage, l_range, l_speed, l_splashDamage, l_splashRange, l_level );
			CT_EQUAL( l_ecs.GetComponentData< TowerState >( l_splashTower, l_ecs.GetComponent( Ecs::StateComponent ) ).GetValue(), TowerState::eIdle );
			CT_EQUAL( l_ecs.GetComponentData< Milliseconds >( l_splashTower, l_ecs.GetComponent( Ecs::CooldownComponent ) ).GetValue(), l_cooldown );
			CT_EQUAL( l_ecs.GetComponentData< uint32_t >( l_splashTower, l_ecs.GetComponent( Ecs::DamageComponent ) ).GetValue(), l_damage );
			CT_EQUAL( l_ecs.GetComponentData< float >( l_splashTower, l_ecs.GetComponent( Ecs::RangeComponent ) ).GetValue(), l_range );
			CT_EQUAL( l_ecs.GetComponentData< uint32_t >( l_splashTower, l_ecs.GetComponent( Ecs::SplashDamageComponent ) ).GetValue(), l_splashDamage );
			CT_EQUAL( l_ecs.GetComponentData< float >( l_splashTower, l_ecs.GetComponent( Ecs::SplashRangeComponent ) ).GetValue(), l_splashRange );
			CT_EQUAL( l_ecs.GetComponentData< float >( l_splashTower, l_ecs.GetComponent( Ecs::SpeedComponent ) ).GetValue(), l_speed );
			CT_EQUAL( l_ecs.GetComponentData< uint32_t >( l_splashTower, l_ecs.GetComponent( Ecs::LevelComponent ) ).GetValue(), l_level );
		}
	}
}
