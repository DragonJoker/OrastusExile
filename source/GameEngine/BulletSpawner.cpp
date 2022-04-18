#include "GameEngine/BulletSpawner.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/WalkData.hpp"
#include "GameEngine/ECS/TrackData.hpp"

#include <Castor3D/Model/Mesh/Mesh.hpp>
#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/Scene.hpp>
#include <Castor3D/Scene/SceneNode.hpp>
#include <Castor3D/Scene/Light/PointLight.hpp>

using namespace castor;
using namespace castor3d;

namespace orastus
{
	BulletSpawner::BulletSpawner( Ecs & ecs
		, Game & game )
		: m_ecs{ ecs }
		, m_game{ game }
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

		for ( auto & bullet : m_bulletsCache )
		{
			auto geometry = m_ecs.getComponentData< GeometrySPtr >( bullet
				, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			Game::getBulletNode( geometry )->setPosition( Point3f{ 0, -1000, 0 } );
		}
	}
	
	void BulletSpawner::fireBullet( Entity source
		, Entity target )
	{
		auto bullet = *m_bulletsCache.begin();
		m_bulletsCache.erase( m_bulletsCache.begin() );
		m_liveBullets.insert( m_liveBullets.end(), bullet );

		auto speed = m_ecs.getComponentData< float >( source
			, m_ecs.getComponent( Ecs::SpeedComponent ) ).getValue();
		auto damage = m_ecs.getComponentData< uint32_t >( source
			, m_ecs.getComponent( Ecs::DamageComponent ) ).getValue();
		auto geometry = m_ecs.getComponentData< GeometrySPtr >( bullet
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
		auto sourceNode = Game::getTowerNode( m_ecs.getComponentData< GeometrySPtr >( source
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue() );
		Game::getBulletNode( geometry )->setPosition( sourceNode->getPosition() );
		m_ecs.resetBullet( bullet
			, geometry
			, std::make_unique< TrackData >( target, speed, damage ) );
	}

	void BulletSpawner::fireBullet( Entity source
		, Entity target
		, castor3d::GeometrySPtr geometry )
	{
		auto speed = m_ecs.getComponentData< float >( source
			, m_ecs.getComponent( Ecs::SpeedComponent ) ).getValue();
		auto damage = m_ecs.getComponentData< uint32_t >( source
			, m_ecs.getComponent( Ecs::DamageComponent ) ).getValue();
		auto sourceNode = Game::getTowerNode( m_ecs.getComponentData< GeometrySPtr >( source
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue() );
		Game::getBulletNode( geometry )->setPosition( sourceNode->getPosition() );
		auto bullet = m_ecs.createBullet( geometry
			, std::make_unique< TrackData >( target, speed, damage ) );
		m_liveBullets.insert( m_liveBullets.end(), bullet );
		++m_totalSpawned;
	}

	void BulletSpawner::killBullet( Entity bullet )
	{
		auto it = std::find( std::begin( m_liveBullets )
			, std::end( m_liveBullets )
			, bullet );

		if ( it != std::end( m_liveBullets ) )
		{
			m_liveBullets.erase( it );
		}

		m_bulletsCache.push_back( bullet );
		auto geometry = m_ecs.getComponentData< GeometrySPtr >( bullet
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
		Game::getBulletNode( geometry )->setPosition( Point3f{ 0, -1000, 0 } );
	}
}
