#include "GameEngine/ECS/Ecs.hpp"

#include "GameEngine/ECS/Bullet.hpp"
#include "GameEngine/ECS/Enemy.hpp"
#include "GameEngine/ECS/MapBlock.hpp"
#include "GameEngine/ECS/Tower.hpp"
#include "GameEngine/ECS/SplashTower.hpp"

using namespace castor;
using namespace castor3d;

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
		String const TRACK_COMPONENT_DESC = cuT( "Track" );
	}

	ComponentId const Ecs::StateComponent = Ecs::hash( "state   " );
	ComponentId const Ecs::CooldownComponent = Ecs::hash( "cooldown" );
	ComponentId const Ecs::DamageComponent = Ecs::hash( "damage  " );
	ComponentId const Ecs::RangeComponent = Ecs::hash( "range   " );
	ComponentId const Ecs::SplashDamageComponent = Ecs::hash( "splshdmg" );
	ComponentId const Ecs::SplashRangeComponent = Ecs::hash( "splshrng" );
	ComponentId const Ecs::LevelComponent = Ecs::hash( "rqurdlvl" );
	ComponentId const Ecs::SpeedComponent = Ecs::hash( "speed   " );
	ComponentId const Ecs::LifeComponent = Ecs::hash( "life    " );
	ComponentId const Ecs::PositionComponent = Ecs::hash( "position" );
	ComponentId const Ecs::GeometryComponent = Ecs::hash( "geometry" );
	ComponentId const Ecs::PickableComponent = Ecs::hash( "pickable" );
	ComponentId const Ecs::AnimationComponent = Ecs::hash( "animtion" );
	ComponentId const Ecs::WalkComponent = Ecs::hash( "walk    " );
	ComponentId const Ecs::AttackComponent = Ecs::hash( "attack  " );
	ComponentId const Ecs::TrackComponent = Ecs::hash( "track   " );

	Ecs::Ecs()
		: m_stateSystem{ *this }
	{
		doRegisterComponents();
		doCreateAssemblages();
	}

	void Ecs::reset()
	{
		m_entities.clear();
		m_entitiesComponents.clear();
	}

	void Ecs::update( Game & p_game
		, Milliseconds const & p_elapsed )
	{
		m_stateSystem.update( p_game, p_elapsed );
	}

	Component const & Ecs::getComponent( ComponentId const & p_name )const
	{
		auto const l_it = std::find_if( std::begin( m_components )
			, std::end( m_components )
			, [&p_name]( Component const & p_component )
		{
			return p_component.getName() == p_name;
		} );

		if ( l_it == std::end( m_components ) )
		{
			throw Component::NameException{ p_name };
		}

		return *l_it;
	}

	Entity Ecs::createMapBlock( castor3d::GeometrySPtr p_geometry )
	{
		auto l_entity = doCreateEntity( "MapBlock" );
		m_mapBlockSet->createData( l_entity
			, p_geometry );
		return l_entity;
	}

	Entity Ecs::createTower( Milliseconds const & p_cooldown
		, uint32_t p_damage
		, float p_range
		, float p_bulletSpeed
		, uint32_t p_requiredLevel
		, GeometrySPtr p_geometry
		, AnimationDataPtr p_animation
		, AttackDataPtr p_attack )
	{
		auto l_entity = doCreateEntity( "Tower" );
		m_towerSet->createData( l_entity
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

	Entity Ecs::createTower( Milliseconds const & p_cooldown
		, uint32_t p_damage
		, float p_range
		, float p_bulletSpeed
		, uint32_t p_splashDamage
		, float p_splashRange
		, uint32_t p_requiredLevel
		, GeometrySPtr p_geometry
		, AnimationDataPtr p_animation
		, AttackDataPtr p_attack )
	{
		auto l_entity = doCreateEntity( "SplashTower" );
		m_splashTowerSet->createData( l_entity
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

	Entity Ecs::createEnemy( float p_speed
		, uint32_t p_life
		, castor3d::GeometrySPtr p_geometry
		, WalkDataPtr p_walkData )
	{
		auto l_entity = doCreateEntity( "Enemy" );
		m_enemySet->createData( l_entity
			, p_speed
			, p_life
			, p_geometry
			, p_walkData );
		return l_entity;
	}

	void Ecs::resetEnemy( Entity p_entity
		, float p_speed
		, uint32_t p_life
		, castor3d::GeometrySPtr p_geometry
		, WalkDataPtr p_walkData )
	{
		m_enemySet->resetData( p_entity
			, p_speed
			, p_life
			, p_geometry
			, p_walkData );
	}

	Entity Ecs::createBullet( castor3d::GeometrySPtr p_geometry
		, TrackDataPtr p_track )
	{
		auto l_entity = doCreateEntity( "Bullet" );
		m_bulletSet->createData( l_entity
			, p_geometry
			, p_track );
		return l_entity;
	}

	void Ecs::resetBullet( Entity p_entity
		, castor3d::GeometrySPtr p_geometry
		, TrackDataPtr p_track )
	{
		m_bulletSet->resetData( p_entity
			, p_geometry
			, p_track );
	}

	void Ecs::printMapBlock( Entity const & p_entity )const
	{
		auto l_text = m_mapBlockSet->toString( p_entity );
		std::cout << l_text << std::endl;
	}

	void Ecs::printTower( Entity const & p_entity )const
	{
		auto l_text = m_towerSet->toString( p_entity );
		std::cout << l_text << std::endl;
	}

	void Ecs::printSplashTower( Entity const & p_entity )const
	{
		auto l_text = m_splashTowerSet->toString( p_entity );
		std::cout << l_text << std::endl;
	}

	void Ecs::printEnemy( Entity const & p_entity )const
	{
		auto l_text = m_enemySet->toString( p_entity );
		std::cout << l_text << std::endl;
	}

	void Ecs::printBullet( Entity const & p_entity )const
	{
		auto l_text = m_bulletSet->toString( p_entity );
		std::cout << l_text << std::endl;
	}

	bool Ecs::hasComponent( Entity const & p_entity
		, Component const & p_component )const
	{
		auto & l_datas = m_entitiesComponents.find( p_component.getName() )->second;
		auto l_it = std::find_if( std::begin( l_datas )
			, std::end( l_datas )
			, [&p_entity]( EntityComponentData const & p_data )
		{
			return p_data.entity == p_entity;
		} );

		return l_it != std::end( l_datas );
	}

	Entity Ecs::doCreateEntity( xchar const * const p_name )
	{
		m_entities.emplace_back( p_name );
		return m_entities.back();
	}

	void Ecs::doCreateComponent( ComponentId const & p_name, String const & p_desc )
	{
		m_components.emplace_back( p_name, p_desc );
		auto l_id = m_components.back().getName();

		if ( m_entitiesComponents.find( l_id ) != m_entitiesComponents.end() )
		{
			throw DuplicateComponentNameException{ unhash( p_name ) };
		}

		m_entitiesComponents.emplace( l_id, EntityComponentsList{} );
	}

	void Ecs::doRegisterComponents()
	{
		doCreateComponent( StateComponent, STATE_COMPONENT_DESC );
		doCreateComponent( CooldownComponent, COOLDOWN_COMPONENT_DESC );
		doCreateComponent( DamageComponent, DAMAGE_COMPONENT_DESC );
		doCreateComponent( SplashDamageComponent, SPLASH_DAMAGE_COMPONENT_DESC );
		doCreateComponent( RangeComponent, RANGE_COMPONENT_DESC );
		doCreateComponent( SplashRangeComponent, SPLASH_RANGE_COMPONENT_DESC );
		doCreateComponent( LevelComponent, LEVEL_COMPONENT_DESC );
		doCreateComponent( SpeedComponent, SPEED_COMPONENT_DESC );
		doCreateComponent( LifeComponent, LIFE_COMPONENT_DESC );
		doCreateComponent( PositionComponent, POSIION_COMPONENT_DESC );
		doCreateComponent( GeometryComponent, GEOMETRY_COMPONENT_DESC );
		doCreateComponent( PickableComponent, PICKABLE_COMPONENT_DESC );
		doCreateComponent( AnimationComponent, ANIMATION_COMPONENT_DESC );
		doCreateComponent( WalkComponent, WALK_COMPONENT_DESC );
		doCreateComponent( AttackComponent, ATTACK_COMPONENT_DESC );
		doCreateComponent( TrackComponent, TRACK_COMPONENT_DESC );
	}

	void Ecs::doCreateAssemblages()
	{
		m_towerSet = std::make_shared< Tower >( *this );
		m_splashTowerSet = std::make_shared< SplashTower >( *this );
		m_enemySet = std::make_shared< Enemy >( *this );
		m_bulletSet = std::make_shared< Bullet >( *this );
		m_mapBlockSet = std::make_shared< MapBlock >( *this );
	}
}
