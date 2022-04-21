#include "GameEngine/BulletSpawner.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/Sound.hpp"
#include "GameEngine/ECS/Bullet.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/SoundSource.hpp"
#include "GameEngine/ECS/Tower.hpp"

#include <Castor3D/Model/Mesh/Mesh.hpp>
#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/Scene.hpp>
#include <Castor3D/Scene/SceneNode.hpp>

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
		for ( size_t i = 0; i < size_t( AmmoType::eCount ); ++i )
		{
			m_bulletsCaches[i].insert( m_bulletsCaches[i].end()
				, m_liveBullets[i].begin()
				, m_liveBullets[i].end() );
			m_liveBullets[i].clear();

			for ( auto & bullet : m_bulletsCaches[i] )
			{
				auto geometry = m_ecs.getComponentData< BulletData >( bullet
					, m_ecs.getComponent( Ecs::BulletStateComponent ) ).getValue().geometry;
				Game::getBulletNode( geometry )->setPosition( castor::Point3f{ 0, -1000, 0 } );
			}
		}
	}
	
	void BulletSpawner::fireBullet( TowerData const & source
		, Entity target
		, Sound & sound )
	{
		auto & bulletsCache = m_bulletsCaches[size_t( source.category->getAmmoType() )];
		auto & liveBullets = m_liveBullets[size_t( source.category->getAmmoType() )];
		auto bullet = *bulletsCache.begin();
		bulletsCache.erase( bulletsCache.begin() );
		liveBullets.insert( liveBullets.end(), bullet );

		auto geometry = m_ecs.getComponentData< BulletData >( bullet
			, m_ecs.getComponent( Ecs::BulletStateComponent ) ).getValue().geometry;
		auto sourceNode = Game::getTowerNode( source.geometry );
		auto node = Game::getBulletNode( geometry );
		node->setPosition( sourceNode->getPosition() );
		m_ecs.resetBullet( bullet
			, &sound.createSource( *node, false )
			, std::make_unique< TrackData >( target
				, source.category->getBulletSpeed()
				, source.category->getDamage() ) );
	}

	void BulletSpawner::fireBullet( TowerData const & source
		, Entity target
		, Sound & sound
		, castor3d::GeometrySPtr geometry )
	{
		auto & liveBullets = m_liveBullets[size_t( source.category->getAmmoType() )];
		auto sourceNode = Game::getTowerNode( source.geometry );
		auto node = Game::getBulletNode( geometry );
		node->setPosition( sourceNode->getPosition() );
		auto bullet = m_ecs.createBullet( source.category->getAmmoType()
			, geometry
			, &sound.createSource( *node, false )
			, std::make_unique< TrackData >( target
				, source.category->getBulletSpeed()
				, source.category->getDamage() ) );
		liveBullets.insert( liveBullets.end(), bullet );
		++m_totalSpawned;
	}

	void BulletSpawner::killBullet( BulletData const & bullet )
	{
		auto & bulletsCache = m_bulletsCaches[size_t( bullet.type )];
		auto & liveBullets = m_liveBullets[size_t( bullet.type )];
		auto it = std::find( std::begin( liveBullets )
			, std::end( liveBullets )
			, bullet.entity );

		if ( it != std::end( liveBullets ) )
		{
			liveBullets.erase( it );
		}

		bulletsCache.push_back( bullet.entity );
		Game::getBulletNode( bullet.geometry )->setPosition( castor::Point3f{ 0, -1000, 0 } );
	}
}
