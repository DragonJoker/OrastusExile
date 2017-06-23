#include "Bullet.hpp"

#include "ECS/Ecs.hpp"

#include <Scene/Geometry.hpp>

namespace orastus
{
	Bullet::Bullet( Ecs & p_ecs )
		: m_ecs{ p_ecs }
		, m_speed{ m_ecs.GetComponent( Ecs::SpeedComponent ) }
		, m_damage{ m_ecs.GetComponent( Ecs::DamageComponent ) }
		, m_geometry{ m_ecs.GetComponent( Ecs::GeometryComponent ) }
	{
	}

	void Bullet::CreateData( Entity const & p_entity
		, float p_speed
		, uint32_t p_damage
		, Castor3D::GeometrySPtr p_geometry )
	{
		m_ecs.CreateComponentData( p_entity
			, m_speed
			, p_speed );
		m_ecs.CreateComponentData( p_entity
			, m_damage
			, p_damage );
		m_ecs.CreateComponentData( p_entity
			, m_geometry
			, p_geometry );
	}

	void Bullet::ResetData( Entity const & p_entity
		, float p_speed
		, uint32_t p_damage
		, Castor3D::GeometrySPtr p_geometry )
	{
		m_ecs.GetComponentData< float >( p_entity
			, m_speed ).SetValue( p_speed );
		m_ecs.GetComponentData< uint32_t >( p_entity
			, m_damage ).SetValue( p_damage );
		m_ecs.GetComponentData< Castor3D::GeometrySPtr >( p_entity
			, m_geometry ).SetValue( p_geometry );
	}

	String Bullet::ToString( Entity const & p_entity )
	{
		StringStream l_stream;
		l_stream << cuT( "Bullet(" ) << p_entity.GetId() << cuT( ")" );
		l_stream << cuT( "\n Speed: " ) << m_ecs.GetComponentData< float >( p_entity, m_speed ).GetValue();
		l_stream << cuT( "\n Damage: " ) << m_ecs.GetComponentData< uint32_t >( p_entity, m_damage ).GetValue();
		l_stream << cuT( "\n Geometry: " ) << m_ecs.GetComponentData< Castor3D::GeometrySPtr >( p_entity, m_geometry ).GetValue()->GetName();
		return l_stream.str();
	}
}
