#include "GameEngine/ECS/MapBlock.hpp"

#include "GameEngine/GridCell.hpp"
#include "GameEngine/ECS/Ecs.hpp"

#include <Castor3D/Scene/Geometry.hpp>

namespace orastus
{
	MapBlock::MapBlock( Ecs & ecs )
		: m_ecs{ ecs }
		, m_cell{ m_ecs.getComponent( Ecs::CellComponent ) }
		, m_geometry{ m_ecs.getComponent( Ecs::GeometryComponent ) }
		, m_pickable{ m_ecs.getComponent( Ecs::PickableComponent ) }
	{
	}

	void MapBlock::createData( Entity const & entity
		, GridCell & cell
		, castor3d::GeometryRPtr geometry
		, bool pickable )
	{
		m_ecs.createComponentData( entity, m_cell, &cell );
		m_ecs.createComponentData( entity, m_geometry, geometry );
		m_ecs.createComponentData( entity, m_pickable, pickable );
	}

	String MapBlock::toString( Entity const & entity )
	{
		auto stream = castor::makeStringStream();
		stream << cuT( "MapBlock(" ) << entity.getId() << cuT( ")" );
		stream << cuT( "\n  Geometry: " ) << m_ecs.getComponentData< castor3d::GeometryRPtr >( entity, m_geometry ).getValue()->getName();
		stream << cuT( "\n  Pickable: " ) << m_ecs.getComponentData< bool >( entity, m_pickable ).getValue();
		return stream.str();
	}
}
