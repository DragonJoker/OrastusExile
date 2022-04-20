#include "GameEngine/BulletSpawner.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/Sound.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/SoundSource.hpp"
#include "GameEngine/ECS/Tower.hpp"
#include "GameEngine/ECS/TrackData.hpp"
#include "GameEngine/ECS/WalkData.hpp"

#include <Castor3D/Model/Mesh/Mesh.hpp>
#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/Scene.hpp>
#include <Castor3D/Scene/SceneNode.hpp>
#include <Castor3D/Scene/Light/PointLight.hpp>

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
			auto geometry = m_ecs.getComponentData< castor3d::GeometrySPtr >( bullet
				, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			Game::getBulletNode( geometry )->setPosition( castor::Point3f{ 0, -1000, 0 } );
		}
	}
	
	void BulletSpawner::fireBullet( TowerData const & source
		, Entity target
		, Sound & sound )
	{
		auto bullet = *m_bulletsCache.begin();
		m_bulletsCache.erase( m_bulletsCache.begin() );
		m_liveBullets.insert( m_liveBullets.end(), bullet );

		auto geometry = m_ecs.getComponentData< castor3d::GeometrySPtr >( bullet
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
		auto sourceNode = Game::getTowerNode( source.geometry );
		auto node = Game::getBulletNode( geometry );
		node->setPosition( sourceNode->getPosition() );
		m_ecs.resetBullet( bullet
			, geometry
			, &sound.createSource( *node, false )
			, std::make_unique< TrackData >( target, source.bulletSpeed, source.damage ) );
	}

	void BulletSpawner::fireBullet( TowerData const & source
		, Entity target
		, Sound & sound
		, castor3d::GeometrySPtr geometry )
	{
		auto sourceNode = Game::getTowerNode( source.geometry );
		auto node = Game::getBulletNode( geometry );
		node->setPosition( sourceNode->getPosition() );
		auto bullet = m_ecs.createBullet( geometry
			, &sound.createSource( *node, false )
			, std::make_unique< TrackData >( target, source.bulletSpeed, source.damage ) );
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
		auto geometry = m_ecs.getComponentData< castor3d::GeometrySPtr >( bullet
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
		Game::getBulletNode( geometry )->setPosition( castor::Point3f{ 0, -1000, 0 } );
	}
}
