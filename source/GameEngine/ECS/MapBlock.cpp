#include "MapBlock.hpp"

#include "ECS/Ecs.hpp"

#include <Castor3D/Scene/Geometry.hpp>

namespace orastus
{
	MapBlock::MapBlock( Ecs & p_ecs )
		: m_ecs{ p_ecs }
		, m_geometry{ p_ecs.getComponent( Ecs::GeometryComponent ) }
		, m_pickable{ p_ecs.getComponent( Ecs::PickableComponent ) }
	{
	}

	void MapBlock::createData( Entity const & p_entity
		, castor3d::GeometrySPtr p_geometry )
	{
		m_ecs.createComponentData( p_entity, m_geometry, p_geometry );
		m_ecs.createComponentData( p_entity, m_pickable, true );
	}

	String MapBlock::toString( Entity const & p_entity )
	{
		StringStream l_stream;
		l_stream << cuT( "MapBlock(" ) << p_entity.getId() << cuT( ")" );
		l_stream << cuT( "\n  Geometry: " ) << m_ecs.getComponentData< castor3d::GeometrySPtr >( p_entity, m_geometry ).getValue()->getName();
		l_stream << cuT( "\n  Pickable: " ) << m_ecs.getComponentData< bool >( p_entity, m_pickable ).getValue();
		return l_stream.str();
	}
}
