#include "MapBlock.hpp"

#include "ECS/Ecs.hpp"

#include <Scene/Geometry.hpp>

namespace orastus
{
	MapBlock::MapBlock( Ecs & p_ecs )
		: m_ecs{ p_ecs }
		, m_geometry{ p_ecs.GetComponent( Ecs::GeometryComponent ) }
		, m_pickable{ p_ecs.GetComponent( Ecs::PickableComponent ) }
	{
	}

	void MapBlock::CreateData( Entity const & p_entity
		, Castor3D::GeometrySPtr p_geometry )
	{
		m_ecs.CreateComponentData( p_entity, m_geometry, p_geometry );
		m_ecs.CreateComponentData( p_entity, m_pickable, true );
	}

	String MapBlock::ToString( Entity const & p_entity )
	{
		StringStream l_stream;
		l_stream << cuT( "MapBlock(" ) << p_entity.GetId() << cuT( ")" );
		l_stream << cuT( "\n  Geometry: " ) << m_ecs.GetComponentData< Castor3D::GeometrySPtr >( p_entity, m_geometry ).GetValue()->GetName();
		l_stream << cuT( "\n  Pickable: " ) << m_ecs.GetComponentData< bool >( p_entity, m_pickable ).GetValue();
		return l_stream.str();
	}
}
