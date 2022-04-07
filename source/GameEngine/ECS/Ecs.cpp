#include "GameEngine/ECS/Ecs.hpp"

#include "GameEngine/ECS/Bullet.hpp"
#include "GameEngine/ECS/Enemy.hpp"
#include "GameEngine/ECS/MapBlock.hpp"
#include "GameEngine/ECS/Tower.hpp"
#include "GameEngine/ECS/SplashTower.hpp"

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

	void Ecs::update( Game & game
		, Milliseconds const & elapsed )
	{
		m_stateSystem.update( game, elapsed );
	}

	Component const & Ecs::getComponent( ComponentId const & name )const
	{
		auto const it = std::find_if( std::begin( m_components )
			, std::end( m_components )
			, [&name]( Component const & lookup )
		{
			return lookup.getName() == name;
		} );

		if ( it == std::end( m_components ) )
		{
			throw Component::NameException{ name };
		}

		return *it;
	}

	Entity Ecs::createMapBlock( castor3d::GeometrySPtr geometry
		, bool pickable )
	{
		auto entity = doCreateEntity( "MapBlock" );
		m_mapBlockSet->createData( entity
			, geometry
			, pickable );
		return entity;
	}

	Entity Ecs::createTower( Milliseconds const & cooldown
		, uint32_t damage
		, float range
		, float bulletSpeed
		, uint32_t requiredLevel
		, castor3d::GeometrySPtr geometry
		, AnimationDataPtr animation
		, AttackDataPtr attack )
	{
		auto entity = doCreateEntity( "Tower" );
		m_towerSet->createData( entity
			, cooldown
			, damage
			, range
			, bulletSpeed
			, requiredLevel
			, geometry
			, animation
			, attack );
		return entity;
	}

	Entity Ecs::createTower( Milliseconds const & cooldown
		, uint32_t damage
		, float range
		, float bulletSpeed
		, uint32_t splashDamage
		, float splashRange
		, uint32_t requiredLevel
		, castor3d::GeometrySPtr geometry
		, AnimationDataPtr animation
		, AttackDataPtr attack )
	{
		auto entity = doCreateEntity( "SplashTower" );
		m_splashTowerSet->createData( entity
			, cooldown
			, damage
			, range
			, bulletSpeed
			, splashDamage
			, splashRange
			, requiredLevel
			, geometry
			, animation
			, attack );
		return entity;
	}

	Entity Ecs::createEnemy( float speed
		, uint32_t life
		, castor3d::GeometrySPtr geometry
		, WalkDataPtr walkData )
	{
		auto entity = doCreateEntity( "Enemy" );
		m_enemySet->createData( entity
			, speed
			, life
			, geometry
			, walkData );
		return entity;
	}

	void Ecs::resetEnemy( Entity entity
		, float speed
		, uint32_t life
		, castor3d::GeometrySPtr geometry
		, WalkDataPtr walkData )
	{
		m_enemySet->resetData( entity
			, speed
			, life
			, geometry
			, walkData );
	}

	Entity Ecs::createBullet( castor3d::GeometrySPtr geometry
		, TrackDataPtr track )
	{
		auto entity = doCreateEntity( "Bullet" );
		m_bulletSet->createData( entity
			, geometry
			, track );
		return entity;
	}

	void Ecs::resetBullet( Entity entity
		, castor3d::GeometrySPtr geometry
		, TrackDataPtr track )
	{
		m_bulletSet->resetData( entity
			, geometry
			, track );
	}

	void Ecs::printMapBlock( Entity const & entity )const
	{
		auto text = m_mapBlockSet->toString( entity );
		std::cout << text << std::endl;
	}

	void Ecs::printTower( Entity const & entity )const
	{
		auto text = m_towerSet->toString( entity );
		std::cout << text << std::endl;
	}

	void Ecs::printSplashTower( Entity const & entity )const
	{
		auto text = m_splashTowerSet->toString( entity );
		std::cout << text << std::endl;
	}

	void Ecs::printEnemy( Entity const & entity )const
	{
		auto text = m_enemySet->toString( entity );
		std::cout << text << std::endl;
	}

	void Ecs::printBullet( Entity const & entity )const
	{
		auto text = m_bulletSet->toString( entity );
		std::cout << text << std::endl;
	}

	bool Ecs::hasComponent( Entity const & entity
		, Component const & component )const
	{
		auto & datas = m_entitiesComponents.find( component.getName() )->second;
		auto it = std::find_if( std::begin( datas )
			, std::end( datas )
			, [&entity]( EntityComponentData const & lookup )
		{
			return lookup.entity == entity;
		} );

		return it != std::end( datas );
	}

	Entity Ecs::doCreateEntity( xchar const * const name )
	{
		m_entities.emplace_back( name );
		return m_entities.back();
	}

	void Ecs::doCreateComponent( ComponentId const & name, String const & desc )
	{
		m_components.emplace_back( name, desc );
		auto id = m_components.back().getName();

		if ( m_entitiesComponents.find( id ) != m_entitiesComponents.end() )
		{
			throw DuplicateComponentNameException{ unhash( name ) };
		}

		m_entitiesComponents.emplace( id, EntityComponentsList{} );
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
