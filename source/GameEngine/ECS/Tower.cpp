#include "GameEngine/ECS/Tower.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/SoundSource.hpp"

#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/Animation/AnimatedObjectGroup.hpp>

namespace orastus
{
	//*********************************************************************************************

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
		, TowerType ptype
		, AttackDataPtr pattack
		, castor3d::GeometrySPtr pgeometry
		, float prange
		, float pbulletSpeed
		, uint32_t pdamage
		, Milliseconds pcooldown
		, AnimationDataPtr panim )
		: entity{ pentity }
		, type{ ptype }
		, status{ TowerStatus::eIdle }
		, attack{ std::move( pattack ) }
		, geometry{ std::move( pgeometry ) }
		, range{ prange }
		, bulletSpeed{ pbulletSpeed }
		, damage{ pdamage }
		, cooldown{ pcooldown }
		, anim{ std::move( panim ) }
		, timeout{ 0_ms }
	{
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
		, TowerType ptype
		, AttackDataPtr pattack
		, castor3d::GeometrySPtr pgeometry
		, float prange
		, float pbulletSpeed
		, uint32_t pdamage
		, Milliseconds pcooldown
		, Milliseconds ptimeout )
		: entity{ pentity }
		, type{ ptype }
		, status{ TowerStatus::eIdle }
		, attack{ std::move( pattack ) }
		, geometry{ std::move( pgeometry ) }
		, range{ prange }
		, bulletSpeed{ pbulletSpeed }
		, damage{ pdamage }
		, cooldown{ pcooldown }
		, timeout{ ptimeout }
	{
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
		, m_pickable{ m_ecs.getComponent( Ecs::PickableComponent ) }
		, m_soundSource{ m_ecs.getComponent( Ecs::SoundSourceComponent ) }
	{
	}

	void Tower::createData( Entity const & entity
		, TowerType type
		, Milliseconds const & cooldown
		, uint32_t damage
		, float range
		, float bulletSpeed
		, castor3d::GeometrySPtr geometry
		, AnimationDataPtr animation
		, AttackDataPtr attack
		, SoundSource const * shootSound )
	{
		if ( animation )
		{
			m_ecs.createComponentData( entity
				, m_tower
				, TowerData{ entity
					, type
					, std::move( attack )
					, geometry
					, range
					, bulletSpeed
					, damage
					, cooldown
					, std::move( animation ) } );
		}
		else
		{
			m_ecs.createComponentData( entity
				, m_tower
				, TowerData{ entity
					, type
					, std::move( attack )
					, geometry
					, range
					, bulletSpeed
					, damage
					, cooldown
					, 0_ms } );
		}

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
		stream << cuT( "\n  Cooldown: " ) << tower.cooldown.count() << " ms";
		stream << cuT( "\n  Damage: " ) << tower.damage;
		stream << cuT( "\n  Range: " ) << tower.range;
		stream << cuT( "\n  BulletSpeed: " ) << tower.bulletSpeed;
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
