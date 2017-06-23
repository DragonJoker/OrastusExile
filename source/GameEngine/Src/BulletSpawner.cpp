#include "BulletSpawner.hpp"

#include "Game.hpp"
#include "ECS/Ecs.hpp"
#include "ECS/WalkData.hpp"

#include <Mesh/Mesh.hpp>
#include <Scene/Geometry.hpp>
#include <Scene/Scene.hpp>
#include <Scene/SceneNode.hpp>
#include <Scene/Light/PointLight.hpp>

using namespace Castor;
using namespace Castor3D;

namespace orastus
{
	BulletSpawner::BulletSpawner( Ecs & p_ecs
		, Game & p_game )
		: m_ecs{ p_ecs }
		, m_game{ p_game }
	{
	}

	BulletSpawner::~BulletSpawner()
	{
	}

	void BulletSpawner::Reset()
	{
		m_bulletsCache.insert( m_bulletsCache.end()
			, m_liveBullets.begin()
			, m_liveBullets.end() );
		m_liveBullets.clear();

		for ( auto & l_bullet : m_bulletsCache )
		{
			auto l_geometry = m_ecs.GetComponentData< GeometrySPtr >( l_bullet
				, m_ecs.GetComponent( Ecs::GeometryComponent ) ).GetValue();
			l_geometry->GetParent()->SetPosition( Point3r{ 0, -1000, 0 } );
		}
	}
	
	void BulletSpawner::FireBullet( Entity p_source
		, Entity p_target )
	{
		auto l_bullet = *m_bulletsCache.begin();
		m_bulletsCache.erase( m_bulletsCache.begin() );
		m_liveBullets.insert( m_liveBullets.end(), l_bullet );

		auto l_speed = m_ecs.GetComponentData< float >( p_source
			, m_ecs.GetComponent( Ecs::SpeedComponent ) ).GetValue();
		auto l_damage = m_ecs.GetComponentData< uint32_t >( p_source
			, m_ecs.GetComponent( Ecs::DamageComponent ) ).GetValue();
		auto l_geometry = m_ecs.GetComponentData< GeometrySPtr >( l_bullet
			, m_ecs.GetComponent( Ecs::GeometryComponent ) ).GetValue();
		m_ecs.ResetBullet( l_bullet
			, l_speed
			, l_damage
			, l_geometry );
	}

	void BulletSpawner::FireBullet( Entity p_source
		, Entity p_target
		, Castor3D::GeometrySPtr p_geometry )
	{
		auto l_speed = m_ecs.GetComponentData< float >( p_source
			, m_ecs.GetComponent( Ecs::SpeedComponent ) ).GetValue();
		auto l_damage = m_ecs.GetComponentData< uint32_t >( p_source
			, m_ecs.GetComponent( Ecs::DamageComponent ) ).GetValue();
		auto l_bullet = m_ecs.CreateBullet( l_speed
			, l_damage
			, p_geometry );
		m_liveBullets.insert( m_liveBullets.end(), l_bullet );
		++m_totalSpawned;
	}

	void BulletSpawner::KillBullet( Entity p_bullet )
	{
		auto l_it = std::find( std::begin( m_liveBullets )
			, std::end( m_liveBullets )
			, p_bullet );

		if ( l_it != std::end( m_liveBullets ) )
		{
			m_liveBullets.erase( l_it );
		}

		m_bulletsCache.push_back( p_bullet );
	}
}
