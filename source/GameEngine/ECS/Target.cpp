#include "GameEngine/ECS/Target.hpp"

#include "GameEngine/GridCell.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/SoundSource.hpp"
#include "GameEngine/State/TargetState.hpp"
#include "GameEngine/State/StateMachine.hpp"

namespace orastus
{
	Target::Target( Ecs & ecs )
		: m_ecs{ ecs }
		, m_geometry{ m_ecs.getComponent( Ecs::GeometryComponent ) }
		, m_state{ m_ecs.getComponent( Ecs::StateComponent ) }
		, m_status{ m_ecs.getComponent( Ecs::StatusComponent ) }
		, m_cell{ m_ecs.getComponent( Ecs::CellComponent ) }
		, m_capturedSound{ m_ecs.getComponent( Ecs::SoundSourceComponent ) }
	{
	}

	void Target::createData( Entity const & entity
		, castor3d::GeometrySPtr geometry
		, GridCell cell
		, SoundSource targetCapturedSound )
	{
		m_ecs.createComponentData( entity
			, m_status
			, TargetState::eIdle );
		m_ecs.createComponentData( entity
			, m_cell
			, std::move( cell ) );
		m_ecs.createComponentData( entity
			, m_capturedSound
			, std::move( targetCapturedSound ) );

		if ( geometry )
		{
			m_ecs.createComponentData( entity
				, m_geometry
				, geometry );
			auto states = std::make_unique< StateMachine >( target::createIdleState( m_ecs, entity ), false );
			states->addState( target::createCapturingState( m_ecs, entity ) );
			m_ecs.createComponentData( entity
				, m_state
				, std::move( states ) );
		}
	}

	void Target::resetData( Entity const & entity
		, castor3d::GeometrySPtr geometry
		, GridCell cell )
	{
		m_ecs.getComponentData< TargetState >( entity
			, m_status ).setValue( TargetState::eIdle );
		m_ecs.getComponentData< GridCell >( entity
			, m_cell ).setValue( std::move( cell ) );
		m_ecs.getComponentData< StateMachinePtr >( entity
			, m_state ).getValue()->restart();
	}

	String Target::toString( Entity const & entity )
	{
		auto stream = castor::makeStringStream();
		stream << cuT( "Target(" ) << entity.getId() << cuT( ")" );
		stream << cuT( "\n Geometry: " ) << m_ecs.getComponentData< castor3d::GeometrySPtr >( entity, m_geometry ).getValue()->getName();
		stream << cuT( "\n Status: " ) << getName( m_ecs.getComponentData< TargetState >( entity, m_state ).getValue() );
		return stream.str();
	}
}
