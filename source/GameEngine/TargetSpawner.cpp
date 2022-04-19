#include "GameEngine/TargetSpawner.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/ECS/Ecs.hpp"
#include "GameEngine/ECS/SoundSource.hpp"
#include "GameEngine/State/TargetState.hpp"

#include <Castor3D/Cache/SceneNodeCache.hpp>
#include <Castor3D/Model/Mesh/Mesh.hpp>
#include <Castor3D/Scene/Geometry.hpp>
#include <Castor3D/Scene/Scene.hpp>
#include <Castor3D/Scene/SceneNode.hpp>
#include <Castor3D/Scene/Light/PointLight.hpp>

namespace orastus
{
	TargetSpawner::TargetSpawner( Ecs & ecs
		, Game & game )
		: m_ecs{ ecs }
		, m_game{ game }
		, m_targetMesh{ m_game.getScene().getMeshCache().find( cuT( "Cow" ) ).lock() }
	{
		reset();
	}

	void TargetSpawner::reset()
	{
		m_liveTargets.clear();
	}

	void TargetSpawner::spawn( GridCell cell )
	{
		if ( m_targetsCache.empty() )
		{
			auto & scene = m_game.getScene();
			auto name = cuT( "Target" ) + castor::string::toString( ++m_count );
			auto node = scene.getSceneNodeCache().add( name ).lock();
			node->setPosition( m_game.convert( castor::Point2i{ cell.x, cell.y } )
				+ castor::Point3f{ 0, m_game.getCellHeight() / 5.0f, 0 } );
			node->yaw( castor::Angle::fromDegrees( m_distribution( m_game.getRandomEngine() ) ) );
			node->setScale( { 0.05f, 0.05f, 0.05f } );
			node->attachTo( m_game.getMapNode() );
			auto geometry = scene.getGeometryCache().create( name, scene, *node, m_targetMesh );
			createAnimation( geometry, "C4D Animation Take", true, false );
			scene.getGeometryCache().add( geometry );
			m_liveTargets.push_back( m_ecs.createTarget( geometry
				, std::move( cell )
				, SoundSource{ *node
					, m_game.getTargetCapturedSound()
					, false } ) );
		}
		else
		{
			auto entity = m_targetsCache.front();
			m_liveTargets.push_back( entity );
			m_targetsCache.erase( m_targetsCache.begin() );
			auto geometry = m_ecs.getComponentData< castor3d::GeometrySPtr >( entity
				, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
			Game::getTargetNode( geometry )->setPosition( m_game.convert( castor::Point2i{ cell.x, cell.y } )
				+ castor::Point3f{ 0, m_game.getCellHeight() / 5.0f, 0 } );
			m_ecs.resetTarget( entity
				, geometry
				, std::move( cell ) );
		}
	}

	Entity TargetSpawner::selectTarget()
	{
		Entity result{};

		if ( !m_liveTargets.empty() )
		{
			result = m_liveTargets.front();
			m_liveTargets.erase( m_liveTargets.begin() );
			m_selectedTargets.push_back( result );
		}

		return result;
	}

	void TargetSpawner::freeTarget( Entity target )
	{
		auto it = std::find( std::begin( m_selectedTargets )
			, std::end( m_selectedTargets )
			, target );

		if ( it != std::end( m_selectedTargets ) )
		{
			m_selectedTargets.erase( it );
		}

		m_liveTargets.push_back( target );
		auto cell = m_ecs.getComponentData< GridCell >( target
			, m_ecs.getComponent( Ecs::CellComponent ) ).getValue();
		auto geometry = m_ecs.getComponentData< castor3d::GeometrySPtr >( target
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
		Game::getTargetNode( geometry )->setPosition( m_game.convert( castor::Point2i{ cell.x, cell.y } )
			+ castor::Point3f{ 0, m_game.getCellHeight() / 5.0f, 0 } );
	}

	void TargetSpawner::targetBeingCaptured( Entity target )
	{
		m_ecs.getComponentData< TargetState >( target
			, m_ecs.getComponent( Ecs::StatusComponent ) ).setValue( TargetState::eCapturing );
		m_ecs.getComponentData< SoundSource >( target
			, m_ecs.getComponent( Ecs::SoundSourceComponent ) ).getValue().play();
	}

	bool TargetSpawner::targetCaptured( Entity target )
	{
		auto it = std::find( std::begin( m_selectedTargets )
			, std::end( m_selectedTargets )
			, target );

		if ( it != std::end( m_selectedTargets ) )
		{
			m_selectedTargets.erase( it );
			--m_count;
		}

		m_ecs.getComponentData< TargetState >( target
			, m_ecs.getComponent( Ecs::StatusComponent ) ).setValue( TargetState::eCaptured );
		m_targetsCache.push_back( target );
		auto geometry = m_ecs.getComponentData< castor3d::GeometrySPtr >( target
			, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
		Game::getTargetNode( geometry )->setPosition( castor::Point3f{ 0, -1000, 0 } );
		return m_count == 0u;
	}
}
