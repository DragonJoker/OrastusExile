#include "GameEngine/ECS/Enemy.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/SoundSource.hpp"

namespace orastus
{
	//*********************************************************************************************

	EnemyData::EnemyData( Entity pentity
		, float pspeed
		, uint32_t plife
		, castor3d::GeometrySPtr pgeometry
		, WalkDataPtr pwalkData )
		: entity{ pentity }
		, status{ EnemyStatus::eSpawning }
		, speed{ pspeed }
		, life{ plife }
		, geometry{ std::move( pgeometry ) }
		, walkData{ std::move( pwalkData ) }
		, timeout{ 0_ms }
	{
		if ( !walkData )
		{
			throw std::runtime_error{ "No walk data" };
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

	Enemy::Enemy( Ecs & ecs )
		: m_ecs{ ecs }
		, m_enemy{ m_ecs.getComponent( Ecs::EnemyStateComponent ) }
		, m_geometry{ m_ecs.getComponent( Ecs::GeometryComponent ) }
		, m_killSound{ m_ecs.getComponent( Ecs::SoundSourceComponent ) }
	{
	}

	void Enemy::createData( Entity const & entity
		, float speed
		, uint32_t life
		, castor3d::GeometrySPtr geometry
		, WalkDataPtr walkData
		, SoundSource const * killSound )
	{
		m_ecs.createComponentData( entity
			, m_enemy
			, EnemyData{ entity
				, speed
				, life
				, geometry
				, std::move( walkData ) } );
		m_ecs.createComponentData( entity
			, m_geometry
			, std::move( geometry ) );
		m_ecs.createComponentData( entity
			, m_killSound
			, killSound );
	}

	void Enemy::resetData( Entity const & entity
		, float speed
		, uint32_t life
		, castor3d::GeometrySPtr geometry
		, WalkDataPtr walkData )
	{
		auto & data = m_ecs.getComponentData< EnemyData >( entity
			, m_enemy ).getValue();
		data.speed = speed;
		data.life = life;
		data.timeout = 0_ms;
		data.target = {};
		data.status = EnemyStatus::eSpawning;
		*data.walkData = *walkData;
	}

	String Enemy::toString( Entity const & entity )
	{
		auto stream = castor::makeStringStream();
		auto & data = m_ecs.getComponentData< EnemyData >( entity
			, m_enemy ).getValue();
		stream << cuT( "Enemy(" ) << entity.getId() << cuT( ")" );
		stream << cuT( "\n Speed: " ) << data.speed;
		stream << cuT( "\n Life: " ) << data.life;
		stream << cuT( "\n Geometry: " ) << data.geometry->getName();
		stream << cuT( "\n Status: " ) << getName( data.status );
		return stream.str();
	}

	bool isTargetable( Ecs const & ecs
		, Entity const & enemy )
	{
		return isTargetable( ecs.getComponentData< EnemyData >( enemy
			, ecs.getComponent( Ecs::EnemyStateComponent ) ).getValue() );
	}

	bool isTargetable( EnemyData const & enemy )
	{
		return enemy.status != EnemyStatus::eSpawning
			&& enemy.status != EnemyStatus::eEscaping
			&& enemy.status != EnemyStatus::eEscaped
			&& enemy.status != EnemyStatus::eDead
			&& enemy.status != EnemyStatus::eDying
			&& enemy.life;
	}

	//*********************************************************************************************
}
