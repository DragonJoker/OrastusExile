#include "GameEngine/ECS/SplashTower.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/SoundSource.hpp"

#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/Animation/AnimatedObjectGroup.hpp>

namespace orastus
{
	//*********************************************************************************************

	SplashTowerData::SplashTowerData( Entity pentity
		, TowerType ptype
		, AttackDataPtr pattack
		, castor3d::GeometrySPtr pgeometry
		, float prange
		, float pbulletSpeed
		, uint32_t pdamage
		, uint32_t psplashDamage
		, float psplashRange
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
		, splashDamage{ psplashDamage }
		, splashRange{ psplashRange }
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

	SplashTowerData::SplashTowerData( Entity pentity
		, TowerType ptype
		, AttackDataPtr pattack
		, castor3d::GeometrySPtr pgeometry
		, float prange
		, float pbulletSpeed
		, uint32_t pdamage
		, uint32_t psplashDamage
		, float psplashRange
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
		, splashDamage{ psplashDamage }
		, splashRange{ psplashRange }
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

	SplashTower::SplashTower( Ecs & ecs )
		: m_ecs{ ecs }
		, m_splashTower{ m_ecs.getComponent( Ecs::SplashTowerStateComponent ) }
		, m_geometry{ m_ecs.getComponent( Ecs::GeometryComponent ) }
		, m_pickable{ m_ecs.getComponent( Ecs::PickableComponent ) }
		, m_soundSource{ m_ecs.getComponent( Ecs::SoundSourceComponent ) }
	{
	}

	void SplashTower::createData( Entity const & entity
		, TowerType type
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
		if ( animation )
		{
			m_ecs.createComponentData( entity
				, m_splashTower
				, SplashTowerData{ entity
					, type
					, std::move( attack )
					, geometry
					, range
					, bulletSpeed
					, damage
					, splashDamage
					, splashRange
					, cooldown
					, std::move( animation ) } );
		}
		else
		{
			m_ecs.createComponentData( entity
				, m_splashTower
				, SplashTowerData{ entity
					, type
					, std::move( attack )
					, geometry
					, range
					, bulletSpeed
					, damage
					, splashDamage
					, splashRange
					, cooldown
					, 0_ms } );
		}

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

	String SplashTower::toString( Entity const & entity )
	{
		auto stream = castor::makeStringStream();
		auto & tower = m_ecs.getComponentData< SplashTowerData >( entity, m_splashTower ).getValue();
		stream << cuT( "SplashTower(" ) << entity.getId() << cuT( ")" );
		stream << cuT( "\n  Cooldown: " ) << tower.cooldown.count() << " ms";
		stream << cuT( "\n  Damage: " ) << tower.damage;
		stream << cuT( "\n  Range: " ) << tower.range;
		stream << cuT( "\n  BulletSpeed: " ) << tower.bulletSpeed;
		stream << cuT( "\n  SplashDamage: " ) << tower.splashDamage;
		stream << cuT( "\n  SplashRange: " ) << tower.splashRange;
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
