#include "BulletSpawner.hpp"

#include "Game.hpp"
#include "ECS/Ecs.hpp"
#include "ECS/WalkData.hpp"
#include "ECS/TrackData.hpp"

#include <Castor3D/Model/Mesh/Mesh.hpp>
#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/Scene.hpp>
#include <Castor3D/Scene/SceneNode.hpp>
#include <Castor3D/Scene/Light/PointLight.hpp>

using namespace castor;
using namespace castor3d;

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

	void BulletSpawner::reset()
	{
		m_bulletsCache.insert( m_bulletsCache.end()
			, m_liveBullets.begin()
			, m_liveBullets.end() );
		m_liveBullets.clear();

		for ( auto & l_bullet : m_bulletsCache )
		{
			auto l_geometry = m_ecs.getComponentData< GeometrySPtr >( l_bullet
				, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			Game::getBulletNode( l_geometry )->setPosition( Point3f{ 0, -1000, 0 } );
		}
	}
	
	void BulletSpawner::fireBullet( Entity p_source
		, Entity p_target )
	{
		auto l_bullet = *m_bulletsCache.begin();
		m_bulletsCache.erase( m_bulletsCache.begin() );
		m_liveBullets.insert( m_liveBullets.end(), l_bullet );

		auto l_speed = m_ecs.getComponentData< float >( p_source
			, m_ecs.getComponent( Ecs::SpeedComponent ) ).getValue();
		auto l_damage = m_ecs.getComponentData< uint32_t >( p_source
			, m_ecs.getComponent( Ecs::DamageComponent ) ).getValue();
		auto l_geometry = m_ecs.getComponentData< GeometrySPtr >( l_bullet
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
		auto l_sourceNode = Game::getTowerNode( m_ecs.getComponentData< GeometrySPtr >( p_source
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue() );
		Game::getBulletNode( l_geometry )->setPosition( l_sourceNode->getPosition() );
		m_ecs.resetBullet( l_bullet
			, l_geometry
			, std::make_shared< TrackData >( p_target, l_speed, l_damage ) );
	}

	void BulletSpawner::fireBullet( Entity p_source
		, Entity p_target
		, castor3d::GeometrySPtr p_geometry )
	{
		auto l_speed = m_ecs.getComponentData< float >( p_source
			, m_ecs.getComponent( Ecs::SpeedComponent ) ).getValue();
		auto l_damage = m_ecs.getComponentData< uint32_t >( p_source
			, m_ecs.getComponent( Ecs::DamageComponent ) ).getValue();
		auto l_sourceNode = Game::getTowerNode( m_ecs.getComponentData< GeometrySPtr >( p_source
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue() );
		Game::getBulletNode( p_geometry )->setPosition( l_sourceNode->getPosition() );
		auto l_bullet = m_ecs.createBullet( p_geometry
			, std::make_shared< TrackData >( p_target, l_speed, l_damage ) );
		m_liveBullets.insert( m_liveBullets.end(), l_bullet );
		++m_totalSpawned;
	}

	void BulletSpawner::killBullet( Entity p_bullet )
	{
		auto l_it = std::find( std::begin( m_liveBullets )
			, std::end( m_liveBullets )
			, p_bullet );

		if ( l_it != std::end( m_liveBullets ) )
		{
			m_liveBullets.erase( l_it );
		}

		m_bulletsCache.push_back( p_bullet );
		auto l_geometry = m_ecs.getComponentData< GeometrySPtr >( p_bullet
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
		Game::getBulletNode( l_geometry )->setPosition( Point3f{ 0, -1000, 0 } );
	}
}
