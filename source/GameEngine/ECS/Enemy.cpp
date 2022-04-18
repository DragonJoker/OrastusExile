#include "GameEngine/ECS/Enemy.hpp"

#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/WalkData.hpp"
#include "GameEngine/State/EnemyState.hpp"
#include "GameEngine/State/StateMachine.hpp"

namespace orastus
{
	Enemy::Enemy( Ecs & ecs )
		: m_ecs{ ecs }
		, m_speed{ m_ecs.getComponent( Ecs::SpeedComponent ) }
		, m_life{ m_ecs.getComponent( Ecs::LifeComponent ) }
		, m_geometry{ m_ecs.getComponent( Ecs::GeometryComponent ) }
		, m_walk{ m_ecs.getComponent( Ecs::WalkComponent ) }
		, m_state{ m_ecs.getComponent( Ecs::StateComponent ) }
		, m_status{ m_ecs.getComponent( Ecs::StatusComponent ) }
	{
	}

	void Enemy::createData( Entity const & entity
		, float speed
		, uint32_t life
		, castor3d::GeometrySPtr geometry
		, WalkDataPtr walkData )
	{
		m_ecs.createComponentData( entity
			, m_speed
			, speed );
		m_ecs.createComponentData( entity
			, m_life
			, life );
		m_ecs.createComponentData( entity
			, m_walk
			, std::move( walkData ) );
		m_ecs.createComponentData( entity
			, m_status
			, EnemyState::eSpawning );

		if ( geometry )
		{
			m_ecs.createComponentData( entity
				, m_geometry
				, geometry );
			auto states = std::make_unique< StateMachine >( enemy::createSpawningState( m_ecs, entity ), false );
			states->addState( enemy::createWalkingState( m_ecs, entity ) );
			states->addState( enemy::createEscapingState( m_ecs, entity ) );
			m_ecs.createComponentData( entity
				, m_state
				, std::move( states ) );
		}
	}

	void Enemy::resetData( Entity const & entity
		, float speed
		, uint32_t life
		, castor3d::GeometrySPtr geometry
		, WalkDataPtr walkData )
	{
		m_ecs.getComponentData< float >( entity
			, m_speed ).setValue( speed );
		m_ecs.getComponentData< uint32_t >( entity
			, m_life ).setValue( life );
		m_ecs.getComponentData< EnemyState >( entity
			, m_status ).setValue( EnemyState::eSpawning );
		*m_ecs.getComponentData< WalkDataPtr >( entity
			, m_walk ).getValue() = *walkData;
		m_ecs.getComponentData< StateMachinePtr >( entity
			, m_state ).getValue()->restart();
	}

	String Enemy::toString( Entity const & entity )
	{
		auto stream = castor::makeStringStream();
		stream << cuT( "Enemy(" ) << entity.getId() << cuT( ")" );
		stream << cuT( "\n Speed: " ) << m_ecs.getComponentData< float >( entity, m_speed ).getValue();
		stream << cuT( "\n Life: " ) << m_ecs.getComponentData< uint32_t >( entity, m_life ).getValue();
		stream << cuT( "\n  Geometry: " ) << m_ecs.getComponentData< castor3d::GeometrySPtr >( entity, m_geometry ).getValue()->getName();
		//stream << cuT( "\n State: " ) << getName( m_ecs.getComponentData< EnemyState >( entity, m_state ).getValue() );
		return stream.str();
	}
}
