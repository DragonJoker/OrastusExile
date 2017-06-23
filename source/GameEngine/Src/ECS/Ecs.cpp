#include "Ecs.hpp"

#include "ECS/Bullet.hpp"
#include "ECS/Enemy.hpp"
#include "ECS/MapBlock.hpp"
#include "ECS/Tower.hpp"
#include "ECS/SplashTower.hpp"

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
		String const POSIION_COMPONENT_DESC = cuT( "Position" );
		String const GEOMETRY_COMPONENT_DESC = cuT( "Geometry" );
		String const PICKABLE_COMPONENT_DESC = cuT( "Pickable" );
		String const ANIMATION_COMPONENT_DESC = cuT( "Animation" );
		String const WALK_COMPONENT_DESC = cuT( "Walk" );
		String const ATTACK_COMPONENT_DESC = cuT( "Attack" );
	}

	ComponentId const Ecs::StateComponent = Ecs::Hash( "state   " );
	ComponentId const Ecs::CooldownComponent = Ecs::Hash( "cooldown" );
	ComponentId const Ecs::DamageComponent = Ecs::Hash( "damage  " );
	ComponentId const Ecs::RangeComponent = Ecs::Hash( "range   " );
	ComponentId const Ecs::SplashDamageComponent = Ecs::Hash( "splshdmg" );
	ComponentId const Ecs::SplashRangeComponent = Ecs::Hash( "splshrng" );
	ComponentId const Ecs::LevelComponent = Ecs::Hash( "rqurdlvl" );
	ComponentId const Ecs::SpeedComponent = Ecs::Hash( "speed   " );
	ComponentId const Ecs::LifeComponent = Ecs::Hash( "life    " );
	ComponentId const Ecs::PositionComponent = Ecs::Hash( "position" );
	ComponentId const Ecs::GeometryComponent = Ecs::Hash( "geometry" );
	ComponentId const Ecs::PickableComponent = Ecs::Hash( "pickable" );
	ComponentId const Ecs::AnimationComponent = Ecs::Hash( "animtion" );
	ComponentId const Ecs::WalkComponent = Ecs::Hash( "walk    " );
	ComponentId const Ecs::AttackComponent = Ecs::Hash( "attack  " );

	Ecs::Ecs()
		: m_stateSystem{ *this }
	{
		DoRegisterComponents();
		DoCreateAssemblages();
	}

	void Ecs::Reset()
	{
		m_entities.clear();
		m_entitiesComponents.clear();
	}

	void Ecs::Update( Milliseconds const & p_elapsed )
	{
		m_stateSystem.Update( p_elapsed );
	}

	Component const & Ecs::GetComponent( ComponentId const & p_name )const
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

	Entity Ecs::CreateMapBlock( Castor3D::GeometrySPtr p_geometry )
	{
		auto l_entity = DoCreateEntity( "MapBlock" );
		m_mapBlockSet->CreateData( l_entity
			, p_geometry );
		return l_entity;
	}

	Entity Ecs::CreateTower( Milliseconds const & p_cooldown
		, uint32_t p_damage
		, float p_range
		, float p_bulletSpeed
		, uint32_t p_requiredLevel
		, GeometrySPtr p_geometry
		, AnimationDataPtr p_animation
		, TowerAttackDataPtr p_attack )
	{
		auto l_entity = DoCreateEntity( "Tower" );
		m_towerSet->CreateData( l_entity
			, p_cooldown
			, p_damage
			, p_range
			, p_bulletSpeed
			, p_requiredLevel
			, p_geometry
			, p_animation
			, p_attack );
		return l_entity;
	}

	Entity Ecs::CreateTower( Milliseconds const & p_cooldown
		, uint32_t p_damage
		, float p_range
		, float p_bulletSpeed
		, uint32_t p_splashDamage
		, float p_splashRange
		, uint32_t p_requiredLevel
		, GeometrySPtr p_geometry
		, AnimationDataPtr p_animation
		, TowerAttackDataPtr p_attack )
	{
		auto l_entity = DoCreateEntity( "SplashTower" );
		m_splashTowerSet->CreateData( l_entity
			, p_cooldown
			, p_damage
			, p_range
			, p_bulletSpeed
			, p_splashDamage
			, p_splashRange
			, p_requiredLevel
			, p_geometry
			, p_animation
			, p_attack );
		return l_entity;
	}

	Entity Ecs::CreateEnemy( float p_speed
		, uint32_t p_life
		, Castor3D::GeometrySPtr p_geometry
		, WalkDataPtr p_walkData )
	{
		auto l_entity = DoCreateEntity( "Enemy" );
		m_enemySet->CreateData( l_entity
			, p_speed
			, p_life
			, p_geometry
			, p_walkData );
		return l_entity;
	}

	void Ecs::ResetEnemy( Entity p_entity
		, float p_speed
		, uint32_t p_life
		, Castor3D::GeometrySPtr p_geometry
		, WalkDataPtr p_walkData )
	{
		m_enemySet->ResetData( p_entity
			, p_speed
			, p_life
			, p_geometry
			, p_walkData );
	}

	Entity Ecs::CreateBullet( float p_speed
		, uint32_t p_damage
		, Castor3D::GeometrySPtr p_geometry )
	{
		auto l_entity = DoCreateEntity( "Bullet" );
		m_bulletSet->CreateData( l_entity
			, p_speed
			, p_damage
			, p_geometry );
		return l_entity;
	}

	void Ecs::ResetBullet( Entity p_entity
		, float p_speed
		, uint32_t p_damage
		, Castor3D::GeometrySPtr p_geometry )
	{
		m_bulletSet->ResetData( p_entity
			, p_speed
			, p_damage
			, p_geometry );
	}

	void Ecs::PrintMapBlock( Entity const & p_entity )const
	{
		auto l_text = m_mapBlockSet->ToString( p_entity );
		std::cout << l_text << std::endl;
	}

	void Ecs::PrintTower( Entity const & p_entity )const
	{
		auto l_text = m_towerSet->ToString( p_entity );
		std::cout << l_text << std::endl;
	}

	void Ecs::PrintSplashTower( Entity const & p_entity )const
	{
		auto l_text = m_splashTowerSet->ToString( p_entity );
		std::cout << l_text << std::endl;
	}

	void Ecs::PrintEnemy( Entity const & p_entity )const
	{
		auto l_text = m_enemySet->ToString( p_entity );
		std::cout << l_text << std::endl;
	}

	void Ecs::PrintBullet( Entity const & p_entity )const
	{
		auto l_text = m_bulletSet->ToString( p_entity );
		std::cout << l_text << std::endl;
	}

	bool Ecs::HasComponent( Entity const & p_entity
		, Component const & p_component )const
	{
		auto & l_datas = m_entitiesComponents.find( p_component.GetName() )->second;
		auto l_it = std::find_if( std::begin( l_datas )
			, std::end( l_datas )
			, [&p_entity, &p_component]( EntityComponentData const & p_data )
		{
			return p_data.m_entity == p_entity;
		} );

		return l_it != std::end( l_datas );
	}

	Entity Ecs::DoCreateEntity( xchar const * const p_name )
	{
		m_entities.emplace_back( p_name );
		return m_entities.back();
	}

	void Ecs::DoCreateComponent( ComponentId const & p_name, String const & p_desc )
	{
		m_components.emplace_back( p_name, p_desc );
		auto l_id = m_components.back().GetName();

		if ( m_entitiesComponents.find( l_id ) != m_entitiesComponents.end() )
		{
			throw DuplicateComponentNameException{ Unhash( p_name ) };
		}

		m_entitiesComponents.emplace( l_id, EntityComponentsList{} );
	}

	void Ecs::DoRegisterComponents()
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
		DoCreateComponent( PositionComponent, POSIION_COMPONENT_DESC );
		DoCreateComponent( GeometryComponent, GEOMETRY_COMPONENT_DESC );
		DoCreateComponent( PickableComponent, PICKABLE_COMPONENT_DESC );
		DoCreateComponent( AnimationComponent, ANIMATION_COMPONENT_DESC );
		DoCreateComponent( WalkComponent, WALK_COMPONENT_DESC );
		DoCreateComponent( AttackComponent, ATTACK_COMPONENT_DESC );
	}

	void Ecs::DoCreateAssemblages()
	{
		m_towerSet = std::make_shared< Tower >( *this );
		m_splashTowerSet = std::make_shared< SplashTower >( *this );
		m_enemySet = std::make_shared< Enemy >( *this );
		m_bulletSet = std::make_shared< Bullet >( *this );
		m_mapBlockSet = std::make_shared< MapBlock >( *this );
	}
}
