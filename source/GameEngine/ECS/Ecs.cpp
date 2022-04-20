#include "GameEngine/ECS/Ecs.hpp"

#include "GameEngine/GridCell.hpp"
#include "GameEngine/ECS/AnimationData.hpp"
#include "GameEngine/ECS/AttackData.hpp"
#include "GameEngine/ECS/Bullet.hpp"
#include "GameEngine/ECS/Enemy.hpp"
#include "GameEngine/ECS/MapBlock.hpp"
#include "GameEngine/ECS/SoundSource.hpp"
#include "GameEngine/ECS/SplashTower.hpp"
#include "GameEngine/ECS/Target.hpp"
#include "GameEngine/ECS/Tower.hpp"
#include "GameEngine/ECS/TrackData.hpp"
#include "GameEngine/ECS/WalkData.hpp"

namespace orastus
{
	namespace
	{
		String const STATE_COMPONENT_DESC = cuT( "State" );
		String const TOWER_STATE_COMPONENT_DESC = cuT( "Tower State" );
		String const SPLASH_TOWER_STATE_COMPONENT_DESC = cuT( "Splash Tower State" );
		String const STATUS_COMPONENT_DESC = cuT( "Status" );
		String const TIMEOUT_COMPONENT_DESC = cuT( "Timeout" );
		String const ENTITY_COMPONENT_DESC = cuT( "Entity" );
		String const CELL_COMPONENT_DESC = cuT( "Cell" );
		String const SPEED_COMPONENT_DESC = cuT( "Speed" );
		String const LIFE_COMPONENT_DESC = cuT( "Life" );
		String const POSIION_COMPONENT_DESC = cuT( "Position" );
		String const GEOMETRY_COMPONENT_DESC = cuT( "Geometry" );
		String const PICKABLE_COMPONENT_DESC = cuT( "Pickable" );
		String const WALK_COMPONENT_DESC = cuT( "Walk" );
		String const TRACK_COMPONENT_DESC = cuT( "Track" );
		String const SOUND_SOURCE_COMPONENT_DESC = cuT( "SoundSource" );
	}

	ComponentId const Ecs::StateComponent = Ecs::hash( "state   " );
	ComponentId const Ecs::TowerStateComponent = Ecs::hash( "twstate " );
	ComponentId const Ecs::SplashTowerStateComponent = Ecs::hash( "stwstate" );
	ComponentId const Ecs::StatusComponent = Ecs::hash( "status  " );
	ComponentId const Ecs::TimeoutComponent = Ecs::hash( "timeout " );
	ComponentId const Ecs::EntityComponent = Ecs::hash( "entity  " );
	ComponentId const Ecs::CellComponent = Ecs::hash( "cell    " );
	ComponentId const Ecs::SpeedComponent = Ecs::hash( "speed   " );
	ComponentId const Ecs::LifeComponent = Ecs::hash( "life    " );
	ComponentId const Ecs::PositionComponent = Ecs::hash( "position" );
	ComponentId const Ecs::GeometryComponent = Ecs::hash( "geometry" );
	ComponentId const Ecs::PickableComponent = Ecs::hash( "pickable" );
	ComponentId const Ecs::WalkComponent = Ecs::hash( "walk    " );
	ComponentId const Ecs::TrackComponent = Ecs::hash( "track   " );
	ComponentId const Ecs::SoundSourceComponent = Ecs::hash( "soundsrc" );

	Ecs::Ecs()
		: m_towerSystem{ *this }
		, m_stateSystem{ *this }
		, m_soundSystem{ *this }
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
		m_towerSystem.update( game, elapsed );
		m_stateSystem.update( game, elapsed );
		m_soundSystem.update( game, elapsed );
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

	Entity Ecs::createTower( TowerType type
		, Milliseconds const & cooldown
		, uint32_t damage
		, float range
		, float bulletSpeed
		, castor3d::GeometrySPtr geometry
		, AnimationDataPtr animation
		, AttackDataPtr attack
		, SoundSource const * shootSound )
	{
		auto entity = doCreateEntity( "Tower" );
		m_towerSet->createData( entity
			, type
			, cooldown
			, damage
			, range
			, bulletSpeed
			, geometry
			, std::move( animation )
			, std::move( attack )
			, shootSound );
		return entity;
	}

	Entity Ecs::createTower( TowerType type
		, Milliseconds const & cooldown
		, uint32_t damage
		, float range
		, float bulletSpeed
		, uint32_t splashDamage
		, float splashRange
		, castor3d::GeometrySPtr geometry
		, AnimationDataPtr animation
		, AttackDataPtr attack
		, SoundSource const * shootSound )
	{
		auto entity = doCreateEntity( "SplashTower" );
		m_splashTowerSet->createData( entity
			, type
			, cooldown
			, damage
			, range
			, bulletSpeed
			, splashDamage
			, splashRange
			, geometry
			, std::move( animation )
			, std::move( attack )
			, shootSound );
		return entity;
	}

	Entity Ecs::createEnemy( float speed
		, uint32_t life
		, castor3d::GeometrySPtr geometry
		, WalkDataPtr walkData
		, SoundSource const * killSound )
	{
		auto entity = doCreateEntity( "Enemy" );
		m_enemySet->createData( entity
			, speed
			, life
			, geometry
			, std::move( walkData )
			, killSound );
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
			, std::move( walkData ) );
	}

	Entity Ecs::createTarget( castor3d::GeometrySPtr geometry
		, GridCell cell
		, SoundSource const * targetCapturedSound )
	{
		auto entity = doCreateEntity( "Target" );
		m_targetSet->createData( entity
			, geometry
			, std::move( cell )
			, targetCapturedSound );
		return entity;
	}

	void Ecs::resetTarget( Entity entity
		, castor3d::GeometrySPtr geometry
		, GridCell cell )
	{
		m_targetSet->resetData( entity
			, geometry
			, std::move( cell ) );
	}

	Entity Ecs::createBullet( castor3d::GeometrySPtr geometry
		, SoundSource const * soundSource
		, TrackDataPtr track )
	{
		auto entity = doCreateEntity( "Bullet" );
		m_bulletSet->createData( entity
			, geometry
			, soundSource
			, std::move( track ) );
		return entity;
	}

	void Ecs::resetBullet( Entity entity
		, castor3d::GeometrySPtr geometry
		, SoundSource const * soundSource
		, TrackDataPtr track )
	{
		m_bulletSet->resetData( entity
			, geometry
			, soundSource
			, std::move( track ) );
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
		doCreateComponent( TowerStateComponent, TOWER_STATE_COMPONENT_DESC );
		doCreateComponent( SplashTowerStateComponent, SPLASH_TOWER_STATE_COMPONENT_DESC );
		doCreateComponent( StatusComponent, STATUS_COMPONENT_DESC );
		doCreateComponent( TimeoutComponent, TIMEOUT_COMPONENT_DESC );
		doCreateComponent( EntityComponent, ENTITY_COMPONENT_DESC );
		doCreateComponent( CellComponent, CELL_COMPONENT_DESC );
		doCreateComponent( SpeedComponent, SPEED_COMPONENT_DESC );
		doCreateComponent( LifeComponent, LIFE_COMPONENT_DESC );
		doCreateComponent( PositionComponent, POSIION_COMPONENT_DESC );
		doCreateComponent( GeometryComponent, GEOMETRY_COMPONENT_DESC );
		doCreateComponent( PickableComponent, PICKABLE_COMPONENT_DESC );
		doCreateComponent( WalkComponent, WALK_COMPONENT_DESC );
		doCreateComponent( TrackComponent, TRACK_COMPONENT_DESC );
		doCreateComponent( SoundSourceComponent, SOUND_SOURCE_COMPONENT_DESC );
	}

	void Ecs::doCreateAssemblages()
	{
		m_targetSet = std::make_unique< Target >( *this );
		m_towerSet = std::make_unique< Tower >( *this );
		m_splashTowerSet = std::make_unique< SplashTower >( *this );
		m_enemySet = std::make_unique< Enemy >( *this );
		m_bulletSet = std::make_unique< Bullet >( *this );
		m_mapBlockSet = std::make_unique< MapBlock >( *this );
	}
}
