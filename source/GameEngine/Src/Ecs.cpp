#include "Ecs.hpp"

#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Tower.hpp"
#include "SplashTower.hpp"

using namespace Castor;
using namespace Castor3D;

namespace orastus
{
	namespace
	{
		String const STATE_COMPONENT_DESC = cuT( "State component" );
		String const COOLDOWN_COMPONENT_DESC = cuT( "Cooldown" );
		String const DAMAGE_COMPONENT_DESC = cuT( "Damage" );
		String const RANGE_COMPONENT_DESC = cuT( "Range" );
		String const SPLASH_DAMAGE_COMPONENT_DESC = cuT( "Splash damage" );
		String const SPLASH_RANGE_COMPONENT_DESC = cuT( "Splash range" );
		String const LEVEL_COMPONENT_DESC = cuT( "Level" );
		String const SPEED_COMPONENT_DESC = cuT( "Speed" );
		String const LIFE_COMPONENT_DESC = cuT( "Life" );
	}

	String const Ecs::StateComponent = cuT( "state" );
	String const Ecs::CooldownComponent = cuT( "cooldown" );
	String const Ecs::DamageComponent = cuT( "damage" );
	String const Ecs::RangeComponent = cuT( "range" );
	String const Ecs::SplashDamageComponent = cuT( "splashdamage" );
	String const Ecs::SplashRangeComponent = cuT( "splashrange" );
	String const Ecs::LevelComponent = cuT( "requiredlevel" );
	String const Ecs::SpeedComponent = cuT( "speed" );
	String const Ecs::LifeComponent = cuT( "life" );

	Ecs::Ecs()
	{
		DoCreateComponent( StateComponent, STATE_COMPONENT_DESC );
		DoCreateComponent( CooldownComponent, COOLDOWN_COMPONENT_DESC );
		DoCreateComponent( DamageComponent, DAMAGE_COMPONENT_DESC );
		DoCreateComponent( SplashDamageComponent, SPLASH_DAMAGE_COMPONENT_DESC );
		DoCreateComponent( RangeComponent, RANGE_COMPONENT_DESC );
		DoCreateComponent( SplashRangeComponent, SPLASH_RANGE_COMPONENT_DESC );
		DoCreateComponent( LevelComponent, LEVEL_COMPONENT_DESC );
		DoCreateComponent( SpeedComponent, SPEED_COMPONENT_DESC );
		DoCreateComponent( LifeComponent, LIFE_COMPONENT_DESC );
		m_towerSet = std::make_shared< Tower >( *this );
		m_splashTowerSet = std::make_shared< SplashTower >( *this );
		m_enemySet = std::make_shared< Enemy >( *this );
		m_bulletSet = std::make_shared< Bullet >( *this );
	}

	void Ecs::Reset()
	{
		m_entities.clear();
		m_entitiesComponents.clear();
	}

	void Ecs::Update()
	{
		// Update systems.
	}

	Component const & Ecs::GetComponent( String const & p_name )const
	{
		auto const l_it = std::find_if( std::begin( m_components )
			, std::end( m_components )
			, [&p_name]( Component const & p_component )
		{
			return p_component.GetName() == p_name;
		} );

		if ( l_it == std::end( m_components ) )
		{
			throw Component::NameException{ p_name };
		}

		return *l_it;
	}

	Entity Ecs::CreateTower( Milliseconds const & p_cooldown
		, uint32_t p_damage
		, float p_range
		, float p_bulletSpeed
		, uint32_t p_requiredLevel )
	{
		auto l_entity = DoCreateEntity( "Tower" );
		//DoCreateComponentAndAdd( RenderableComponent, l_entity );
		//DoCreateComponentAndAdd( AnimatableComponent, l_entity );
		m_towerSet->CreateData( l_entity
			, p_cooldown
			, p_damage
			, p_range
			, p_bulletSpeed
			, p_requiredLevel );
		return l_entity;
	}

	Entity Ecs::CreateTower( Milliseconds const & p_cooldown
		, uint32_t p_damage
		, float p_range
		, float p_bulletSpeed
		, uint32_t p_splashDamage
		, float p_splashRange
		, uint32_t p_requiredLevel )
	{
		auto l_entity = DoCreateEntity( "SplashTower" );
		//DoCreateComponentAndAdd( RenderableComponent, l_entity );
		//DoCreateComponentAndAdd( AnimatableComponent, l_entity );
		m_splashTowerSet->CreateData( l_entity
			, p_cooldown
			, p_damage
			, p_range
			, p_bulletSpeed
			, p_splashDamage
			, p_splashRange
			, p_requiredLevel );
		return l_entity;
	}

	Entity Ecs::CreateEnemy( float p_speed
		, uint32_t p_life )
	{
		auto l_entity = DoCreateEntity( "Enemy" );
		//DoCreateComponentAndAdd( RenderableComponent, l_entity );
		//DoCreateComponentAndAdd( AnimatableComponent, l_entity );
		m_enemySet->CreateData( l_entity
			, p_speed
			, p_life );
		return l_entity;
	}

	Entity Ecs::CreateBullet( float p_speed
		, uint32_t p_damage )
	{
		auto l_entity = DoCreateEntity( "Bullet" );
		//DoCreateComponentAndAdd( RenderableComponent, l_entity );
		m_bulletSet->CreateData( l_entity
			, p_speed
			, p_damage );
		return l_entity;
	}

	Entity Ecs::DoCreateEntity( xchar const * const p_name )
	{
		m_entities.emplace_back( p_name );
		return m_entities.back();
	}

	void Ecs::DoCreateComponent( String const & p_name, String const & p_desc )
	{
		m_components.emplace_back( p_name, p_desc );
	}
}
