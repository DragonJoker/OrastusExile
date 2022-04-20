#include "GameEngine/ECS/Target.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/GridCell.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/SoundSource.hpp"

namespace orastus
{
	//*********************************************************************************************

	TargetData::TargetData( Entity pentity
		, castor3d::GeometrySPtr pgeometry
		, GridCell pcell )
		: entity{ pentity }
		, status{ TargetStatus::eIdle }
		, geometry{ std::move( pgeometry ) }
		, cell{ std::move( pcell ) }
	{
		if ( !geometry )
		{
			throw std::runtime_error{ "No geometry" };
		}

		auto node = Game::getTargetNode( geometry );

		if ( !node )
		{
			throw std::runtime_error{ "Geometry is not attached" };
		}
	}

	//*********************************************************************************************

	Target::Target( Ecs & ecs )
		: m_ecs{ ecs }
		, m_target{ m_ecs.getComponent( Ecs::TargetStateComponent ) }
		, m_capturedSound{ m_ecs.getComponent( Ecs::SoundSourceComponent ) }
	{
	}

	void Target::createData( Entity const & entity
		, castor3d::GeometrySPtr geometry
		, GridCell cell
		, SoundSource const * targetCapturedSound )
	{
		m_ecs.createComponentData( entity
			, m_target
			, TargetData{ entity
				, geometry
				, std::move( cell ) } );
		m_ecs.createComponentData( entity
			, m_capturedSound
			, targetCapturedSound );
	}

	void Target::resetData( Entity const & entity
		, GridCell cell )
	{
		auto & data = m_ecs.getComponentData< TargetData >( entity
			, m_target ).getValue();
		data.status = TargetStatus::eIdle;
		data.cell = std::move( cell );
	}

	String Target::toString( Entity const & entity )
	{
		auto stream = castor::makeStringStream();
		auto & data = m_ecs.getComponentData< TargetData >( entity
			, m_target ).getValue();
		stream << cuT( "Target(" ) << entity.getId() << cuT( ")" );
		stream << cuT( "\n Geometry: " ) << data.geometry->getName();
		stream << cuT( "\n Status: " ) << getName( data.status );
		return stream.str();
	}

	//*********************************************************************************************
}
