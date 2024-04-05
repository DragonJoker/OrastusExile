#include "GameEngine/ECS/Tower.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/SoundSource.hpp"

#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/Animation/AnimatedObjectGroup.hpp>

namespace orastus
{
	//*********************************************************************************************

	String getName( TowerType type )
	{
		switch ( type )
		{
		case TowerType::eDirect:
			return cuT( "Direct" );
		case TowerType::eSplash:
			return cuT( "Splash" );
		default:
			CU_Failure( "Unexpected tower type." );
			return "Unknown";
		}
	}

	String getName( TowerStatus state )
	{
		switch ( state )
		{
		case TowerStatus::eEmpty:
			return cuT( "Empty" );
		case TowerStatus::eIdle:
			return cuT( "Idle" );
		case TowerStatus::eShooting:
			return cuT( "Shooting" );
		default:
			CU_Failure( "Unexpected tower state." );
			return "Unknown";
		}
	}

	//*********************************************************************************************

	TowerData::TowerData( Entity pentity
		, TowerCategoryPtr pcategory
		, AttackDataPtr pattack
		, castor3d::GeometryRPtr pgeometry
		, AnimationDataPtr panim )
		: entity{ pentity }
		, category{ std::move( pcategory ) }
		, status{ TowerStatus::eIdle }
		, attack{ std::move( pattack ) }
		, geometry{ std::move( pgeometry ) }
		, anim{ std::move( panim ) }
		, timeout{ 0_ms }
	{
		if ( !category )
		{
			throw std::runtime_error{ "No category" };
		}

		if ( !attack )
		{
			throw std::runtime_error{ "No attack data" };
		}

		if ( !geometry )
		{
			throw std::runtime_error{ "No geometry" };
		}

		auto node = Game::getTowerNode( geometry );

		if ( !node )
		{
			throw std::runtime_error{ "Geometry is not attached" };
		}
	}

	TowerData::TowerData( Entity pentity
		, TowerCategoryPtr pcategory
		, AttackDataPtr pattack
		, castor3d::GeometryRPtr pgeometry
		, Milliseconds ptimeout )
		: entity{ pentity }
		, category{ std::move( pcategory ) }
		, status{ TowerStatus::eIdle }
		, attack{ std::move( pattack ) }
		, geometry{ std::move( pgeometry ) }
		, timeout{ ptimeout }
	{
		if ( !category )
		{
			throw std::runtime_error{ "No category" };
		}

		if ( !attack )
		{
			throw std::runtime_error{ "No attack data" };
		}

		if ( !geometry )
		{
			throw std::runtime_error{ "No geometry" };
		}

		auto node = Game::getTowerNode( geometry );

		if ( !node )
		{
			throw std::runtime_error{ "Geometry is not attached" };
		}
	}

	//*********************************************************************************************

	Tower::Tower( Ecs & ecs )
		: m_ecs{ ecs }
		, m_tower{ m_ecs.getComponent( Ecs::TowerStateComponent ) }
		, m_cell{ m_ecs.getComponent( Ecs::CellComponent ) }
		, m_geometry{ m_ecs.getComponent( Ecs::GeometryComponent ) }
		, m_pickable{ m_ecs.getComponent( Ecs::PickableComponent ) }
		, m_soundSource{ m_ecs.getComponent( Ecs::SoundSourceComponent ) }
	{
	}

	void Tower::createData( Entity const & entity
		, TowerCategoryPtr category
		, GridCell & cell
		, castor3d::GeometryRPtr geometry
		, AnimationDataPtr animation
		, AttackDataPtr attack
		, SoundSource const * shootSound )
	{
		if ( animation )
		{
			m_ecs.createComponentData( entity
				, m_tower
				, TowerData{ entity
					, std::move( category )
					, std::move( attack )
					, geometry
					, std::move( animation ) } );
		}
		else
		{
			m_ecs.createComponentData( entity
				, m_tower
				, TowerData{ entity
				, std::move( category )
					, std::move( attack )
					, geometry
					, 0_ms } );
		}

		m_ecs.createComponentData( entity
			, m_cell
			, &cell );
		m_ecs.createComponentData( entity
			, m_geometry
			, std::move( geometry ) );
		m_ecs.createComponentData( entity
			, m_pickable
			, true );
		m_ecs.createComponentData( entity
			, m_soundSource
			, shootSound );
	}

	String Tower::toString( Entity const & entity )
	{
		auto stream = castor::makeStringStream();
		auto & tower = m_ecs.getComponentData< TowerData >( entity, m_tower ).getValue();
		stream << cuT( "Tower(" ) << entity.getId() << cuT( ")" );
		stream << cuT( "\n  Cooldown: " ) << tower.category->getCooldown().count() << " ms";
		stream << cuT( "\n  Damage: " ) << tower.category->getDamage();
		stream << cuT( "\n  Range: " ) << tower.category->getRange();
		stream << cuT( "\n  BulletSpeed: " ) << tower.category->getBulletSpeed();
		stream << cuT( "\n  Geometry: " ) << tower.geometry->getName();
		stream << cuT( "\n  Pickable: " ) << m_ecs.getComponentData< bool >( entity, m_pickable ).getValue();
		stream << cuT( "\n  Attack: " ) << tower.attack->target.getId();

		if ( tower.anim )
		{
			stream << cuT( "\n  Animation: " ) << tower.anim->animation->getName();
		}

		return stream.str();
	}

	//*********************************************************************************************
}
