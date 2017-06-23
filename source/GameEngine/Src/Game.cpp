#include "Game.hpp"

#include "Ecs/AnimationData.hpp"
#include "Ecs/AttackData.hpp"
#include "ECS/WalkData.hpp"
#include "State/EnemyState.hpp"
#include "State/StateMachine.hpp"
#include "State/TowerState.hpp"

#include <Event/Frame/FrameListener.hpp>
#include <Event/Frame/FunctorEvent.hpp>
#include <Material/Material.hpp>
#include <Mesh/Mesh.hpp>
#include <Mesh/Submesh.hpp>
#include <Mesh/Skeleton/Skeleton.hpp>
#include <Scene/Geometry.hpp>
#include <Scene/Scene.hpp>
#include <Scene/Animation/AnimatedObject.hpp>
#include <Scene/Animation/AnimatedObjectGroup.hpp>
#include <Scene/Light/Light.hpp>
#include <Scene/Light/PointLight.hpp>

using namespace Castor;
using namespace Castor3D;

namespace orastus
{
	//*********************************************************************************************

	namespace
	{
		void DoPrepareGridLine( GridPathNode const & p_prv
			, GridPathNode const & p_cur
			, Grid & p_grid )
		{
			if ( p_prv.m_x != p_cur.m_x )
			{
				for ( auto x = std::min( p_prv.m_x, p_cur.m_x ); x < std::max( p_prv.m_x, p_cur.m_x ); ++x )
				{
					p_grid( p_prv.m_y - 1, x - 1 ).m_state = GridCell::State::ePath;
					p_grid( p_prv.m_y - 1, x - 0 ).m_state = GridCell::State::ePath;
					p_grid( p_prv.m_y - 0, x - 1 ).m_state = GridCell::State::ePath;
					p_grid( p_prv.m_y - 0, x - 0 ).m_state = GridCell::State::ePath;
				}
			}
			else
			{
				for ( auto y = std::min( p_prv.m_y, p_cur.m_y ); y <= std::max( p_prv.m_y, p_cur.m_y ); ++y )
				{
					p_grid( y - 1, p_prv.m_x - 1 ).m_state = GridCell::State::ePath;
					p_grid( y - 1, p_prv.m_x - 0 ).m_state = GridCell::State::ePath;
					p_grid( y - 0, p_prv.m_x - 1 ).m_state = GridCell::State::ePath;
					p_grid( y - 0, p_prv.m_x - 0 ).m_state = GridCell::State::ePath;
				}
			}
		}

		void DoPrepareTarget( GridPathNode const & p_cur
			, Scene & p_scene
			, Grid & p_grid )
		{
			for ( uint32_t x = p_cur.m_x - 2; x <= p_cur.m_x + 2; ++x )
			{
				for ( uint32_t y = p_cur.m_y - 1; y <= p_cur.m_y + 3; ++y )
				{
					p_grid( y, x ).m_state = GridCell::State::ePath;
				}
			}

			p_grid( p_cur.m_y, p_cur.m_x ).m_state = GridCell::State::eTarget;
		}
	}

	//*********************************************************************************************

	void Game::SelectedEntity::Select( Entity const & p_entity
		, Castor3D::GeometrySPtr p_geometry )
	{
		Unselect();

		if ( p_entity && p_geometry )
		{
			m_entity = p_entity;
			m_geometry = p_geometry;
			m_materials.clear();
			auto & l_materials = m_geometry->GetScene()->GetEngine()->GetMaterialCache();

			for ( auto l_submesh : *m_geometry->GetMesh() )
			{
				auto l_old = m_geometry->GetMaterial( *l_submesh );
				auto l_new = l_materials.Find( l_old->GetName() + cuT( "_sel" ) );
				REQUIRE( l_new );
				m_geometry->SetMaterial( *l_submesh, l_new );
				m_materials.push_back( l_old );
			}
		}
	}

	void Game::SelectedEntity::Unselect()
	{
		if ( m_entity && m_geometry )
		{
			uint32_t l_index{ 0u };
			auto l_mesh = m_geometry->GetMesh();

			for ( auto l_material : m_materials )
			{
				m_geometry->SetMaterial( *l_mesh->GetSubmesh( l_index++ ), l_material );
			}

			m_materials.clear();
			m_geometry.reset();
			m_entity = Entity{};
		}
	}

	//*********************************************************************************************

	auto constexpr s_faceIndex = 5u;

	void Game::SelectedBlock::Select( Entity const & p_entity
		, Castor3D::GeometrySPtr p_geometry )
	{
		Unselect();

		if ( p_entity && p_geometry )
		{
			m_entity = p_entity;
			m_geometry = p_geometry;
			auto & l_submesh = *m_geometry->GetMesh()->GetSubmesh( s_faceIndex );
			m_material = p_geometry->GetMaterial( l_submesh );
			auto & l_materials = m_geometry->GetScene()->GetEngine()->GetMaterialCache();
			auto l_new = l_materials.Find( m_material->GetName() + cuT( "_sel" ) );
			m_geometry->SetMaterial( l_submesh, l_new );
		}
	}

	void Game::SelectedBlock::Unselect()
	{
		if ( m_entity && m_geometry )
		{
			uint32_t l_index{ 0u };
			auto & l_submesh = *m_geometry->GetMesh()->GetSubmesh( s_faceIndex );
			m_geometry->SetMaterial( l_submesh, m_material );
			m_material.reset();
			m_geometry.reset();
			m_entity = Entity{};
		}
	}

	//*********************************************************************************************

	Game::Game( Castor3D::Scene & p_scene )
		: m_scene{ p_scene }
		, m_hud{ *this, p_scene }
		, m_path
		{
			{ 19,  1 },
			{ 19,  3 },
			{ 11,  3 },
			{ 11,  7 },
			{ 23,  7 },
			{ 23, 11 },
			{ 13, 11 },
			{ 13, 15 },
			{ 25, 15 },
			{ 25, 19 },
			{ 15, 19 },
			{ 15, 23 },
			{ 17, 23 },
			{ 17, 27 },
		}
		, m_enemySpawner{ m_ecs, *this }
		, m_bulletSpawner{ m_ecs, *this }
	{
		m_mapNode = m_scene.GetSceneNodeCache().Find( cuT( "MapBase" ) );
		m_mapCubeMesh = m_scene.GetMeshCache().Find( cuT( "MapCube" ) );
		m_mapCubeMaterial = m_scene.GetMaterialView().Find( cuT( "MapCube" ) );
		m_shortRangeTowerMesh = m_scene.GetMeshCache().Find( cuT( "ShortRange" ) );
		m_longRangeTowerMesh = m_scene.GetMeshCache().Find( cuT( "HeavySplash" ) );
		m_enemyCubeMesh = m_scene.GetMeshCache().Find( cuT( "EnemyCube" ) );
		m_enemyCubeMaterial = m_scene.GetMaterialView().Find( cuT( "EnemyCube" ) );
		m_bulletMesh = m_scene.GetMeshCache().Find( cuT( "Bullet" ) );
		m_bulletMaterial = m_scene.GetMaterialView().Find( cuT( "Bullet" ) );
		m_targetNode = m_scene.GetSceneNodeCache().Find( cuT( "Target" ) );
		m_cellDimensions[0] = m_mapCubeMesh->GetCollisionBox().GetMax()[0] - m_mapCubeMesh->GetCollisionBox().GetMin()[0];
		m_cellDimensions[1] = m_mapCubeMesh->GetCollisionBox().GetMax()[1] - m_mapCubeMesh->GetCollisionBox().GetMin()[1];
		m_cellDimensions[2] = m_mapCubeMesh->GetCollisionBox().GetMax()[2] - m_mapCubeMesh->GetCollisionBox().GetMin()[2];
		m_hud.Initialise();
		m_state = State::eInitial;
	}

	void Game::Reset()
	{
		Grid l_grid;
		std::swap( m_grid, l_grid );
		m_state = State::eStarted;
		m_bulletSpawner.Reset();
		m_enemySpawner.Reset();
	}

	void Game::Start()
	{
		DoPrepareGrid();

		for ( auto & l_cell : m_grid )
		{
			if ( l_cell.m_state == GridCell::State::eEmpty )
			{
				DoAddMapCube( l_cell );
			}
		}

		for ( auto l_submesh : *m_lastMapCube->GetMesh() )
		{
			// We need to update the render nodes (since the submesh's geometry buffers are now invalid).
			m_scene.SetChanged();
			m_scene.GetListener().PostEvent( MakeFunctorEvent( EventType::eQueueRender
				, [this, l_submesh]()
			{
				// TODO: Find a better way, since this forbids the suppression of RAM storage of the VBO data.
				l_submesh->ResetGpuBuffers();
			} ) );
		}

		auto & l_node = *m_path.rbegin();
		DoAddTarget( m_grid( l_node.m_y, l_node.m_x ) );

		m_state = State::eStarted;
		m_hud.Start();
		m_saved = Clock::now();
	}

	void Game::Pause()
	{
		m_state = State::ePaused;
		m_hud.Pause();
	}

	void Game::Resume()
	{
		m_state = State::eStarted;
		m_hud.Resume();
		m_saved = Clock::now();
	}

	void Game::Help()
	{
		m_state = State::ePaused;
		m_hud.Help();
	}

	void Game::Update()
	{
		if ( IsRunning() )
		{
#if !defined( NDEBUG )
			m_elapsed = std::chrono::milliseconds{ 40 };
#else
			m_elapsed = std::chrono::duration_cast< std::chrono::milliseconds >( Clock::now() - m_saved );
#endif
			m_ecs.Update( *this, m_elapsed );
			m_enemySpawner.Update( m_elapsed );
			m_saved = Clock::now();
		}
	}

	Entity Game::Select( Castor3D::GeometrySPtr p_geometry )
	{
		Entity l_result;
		auto l_entity = m_ecs.FindEntity( m_ecs.GetComponent( Ecs::GeometryComponent ), p_geometry );
		m_selectedCell = &DoFindCell( l_entity );
		l_entity = m_selectedCell->m_entity;

		if ( m_ecs.HasComponent( l_entity, m_ecs.GetComponent( Ecs::PickableComponent ) ) )
		{
			if ( DoSelectEntity( l_entity ) )
			{
				l_result = l_entity;
			}
		}
		else
		{
			Unselect();
		}

		return l_result;
	}

	void Game::Unselect()
	{
		m_hud.HideBuild();
		m_hud.UpdateTowerInfo( m_ecs, Entity{} );
		m_selectedCell = nullptr;

		m_scene.GetEngine()->PostEvent( MakeFunctorEvent( EventType::ePreRender
			, [this]()
			{
				m_selectedBlock.Unselect();
				m_selectedEntity.Unselect();
			} ) );
	}

	void Game::KillEnemy( Entity p_entity )
	{
		m_enemySpawner.KillEnemy( p_entity );
	}

	void Game::Hit( Entity p_source
		, Entity p_target
		, uint32_t p_damage )
	{
		m_bulletSpawner.KillBullet( p_source );

		auto & l_life = m_ecs.GetComponentData< uint32_t >( p_target
			, m_ecs.GetComponent( Ecs::LifeComponent ) );
		l_life.SetValue( l_life.GetValue() - std::min( l_life.GetValue(), p_damage ) );

		if ( !l_life.GetValue() )
		{
			m_enemySpawner.KillEnemy( p_target );
		}
	}

	Castor3D::GeometrySPtr Game::CreateEnemy( Castor::String const & p_name )
	{
		auto l_baseNode = m_scene.GetSceneNodeCache().Add( p_name + cuT( "_Base" ) );
		l_baseNode->AttachTo( m_mapNode );
		auto l_node = m_scene.GetSceneNodeCache().Add( p_name );
		l_node->SetOrientation( Quaternion::from_axis_angle( Point3r{ 1, 0, 1 }, Angle::from_degrees( 45 ) ) );
		l_node->AttachTo( l_baseNode );
		auto l_geometry = m_scene.GetGeometryCache().Add( p_name, l_node, m_enemyCubeMesh );

		for ( auto l_submesh : *l_geometry->GetMesh() )
		{
			l_geometry->SetMaterial( *l_submesh, m_enemyCubeMaterial );
		}

		auto l_light = m_scene.GetLightCache().Add( p_name, l_node, LightType::ePoint );
		l_light->SetColour( Colour::from_predef( PredefinedColour::eOpaqueRed ) );
		l_light->SetIntensity( 0.8f, 1.0f );
		l_light->GetPointLight()->SetAttenuation( Point3f{ 1.0f, 0.1f, 0.0f } );
		return l_geometry;
	}

	void Game::CreateShortRangeTower()
	{
		REQUIRE( m_selectedCell && *m_selectedCell );
		if ( m_selectedCell->m_state == GridCell::State::eEmpty )
		{
			String l_name = cuT( "ShortRangeTower_" ) + std::to_string( m_selectedCell->m_x ) + cuT( "x" ) + std::to_string( m_selectedCell->m_y );
			auto l_tower = DoCreateTower( l_name
				, *m_selectedCell
				, m_shortRangeTowerMesh
				, { cuT( "short_range_accessories" ), cuT( "short_range_accessories" ), cuT( "short_range_body" ) } );
			String l_animName = cuT( "armature_short_range.1|attack" );
			auto l_anim = DoCreateAnimation( l_tower
				, l_animName );

			if ( l_tower )
			{
				auto l_data = std::make_shared< AnimationData >( l_anim, l_animName );
				auto l_shoot = std::make_shared< AttackData >( 500_ms );
				m_selectedCell->m_entity = m_ecs.CreateTower( 1000_ms
					, 3u
					, 40.0f
					, 120.0f
					, 1u
					, l_tower
					, l_data
					, l_shoot );
				auto l_stateMachine = m_ecs.GetComponentData< StateMachinePtr >( m_selectedCell->m_entity
					, m_ecs.GetComponent( Ecs::StateComponent ) ).GetValue();
				l_stateMachine->AddState( tower::CreateShootingState( m_ecs, m_selectedCell->m_entity ) );
				DoSelectEntity( m_selectedCell->m_entity );
			}
		}
	}

	void Game::CreateLongRangeTower()
	{
		REQUIRE( m_selectedCell && *m_selectedCell );
		if ( m_selectedCell->m_state == GridCell::State::eEmpty )
		{
			String l_name = cuT( "LongRangeTower_" ) + std::to_string( m_selectedCell->m_x ) + cuT( "x" ) + std::to_string( m_selectedCell->m_y );
			auto l_tower = DoCreateTower( l_name
				, *m_selectedCell
				, m_longRangeTowerMesh
				, { cuT( "splash_accessories" ), cuT( "splash_accessories" ), cuT( "splash_body" ) } );
			String l_animName = cuT( "armature_short_range.1|attack" );
			auto l_anim = DoCreateAnimation( l_tower
				, l_animName );

			if ( l_tower )
			{
				auto l_data = std::make_shared< AnimationData >( l_anim, l_animName );
				auto l_shoot = std::make_shared< AttackData >( 800_ms );
				m_selectedCell->m_entity = m_ecs.CreateTower( 6000_ms
					, 5u
					, 100.0f
					, 96.0f
					, 1u
					, l_tower
					, l_data
					, l_shoot );
				auto l_stateMachine = m_ecs.GetComponentData< StateMachinePtr >( m_selectedCell->m_entity
					, m_ecs.GetComponent( Ecs::StateComponent ) ).GetValue();
				l_stateMachine->AddState( tower::CreateShootingState( m_ecs, m_selectedCell->m_entity ) );
				DoSelectEntity( m_selectedCell->m_entity );
			}
		}
	}

	void Game::CreateBullet( Entity p_source
		, Entity p_target )
	{
		if ( m_bulletSpawner.HasFreeBullet() )
		{
			m_bulletSpawner.FireBullet( p_source
				, p_target );
		}
		else
		{
			m_bulletSpawner.FireBullet( p_source
				, p_target
				, DoCreateBullet( p_source ) );
		}
	}

	Point3r Game::Convert( Castor::Point2i const & p_position )const
	{
		return Point3r( ( p_position[0] - int( m_grid.GetWidth() ) / 2 ) * m_cellDimensions[0]
			, 0
			, ( p_position[1] - int( m_grid.GetHeight() ) / 2 ) * m_cellDimensions[2] );
	}

	Castor3D::SceneNodeSPtr Game::GetEnemyNode( Castor3D::GeometrySPtr p_geometry )
	{
		return p_geometry->GetParent()->GetParent();
	}

	Castor3D::SceneNodeSPtr Game::GetTowerNode( Castor3D::GeometrySPtr p_geometry )
	{
		return p_geometry->GetParent();
	}

	Castor3D::SceneNodeSPtr Game::GetBulletNode( Castor3D::GeometrySPtr p_geometry )
	{
		return p_geometry->GetParent();
	}

	void Game::DoPrepareGrid()
	{
		auto l_prv = m_path.begin();

		if ( l_prv != m_path.end() )
		{
			auto l_cur = l_prv + 1;

			while ( l_cur != m_path.end() )
			{
				DoPrepareGridLine( *l_prv, *l_cur, m_grid );

				if ( l_prv == m_path.begin() )
				{
					m_grid( l_prv->m_y, l_prv->m_x ).m_state = GridCell::State::eStart;
				}

				++l_prv;
				++l_cur;
			}

			DoPrepareTarget( *l_prv, m_scene, m_grid );
		}
	}

	void Game::DoAddMapCube( GridCell & p_cell )
	{
		String l_name = cuT( "MapCube_" ) + std::to_string( p_cell.m_x ) + cuT( "x" ) + std::to_string( p_cell.m_y );
		auto l_node = m_scene.GetSceneNodeCache().Add( l_name );
		auto l_geometry = m_scene.GetGeometryCache().Add( l_name, l_node, m_mapCubeMesh );
		l_node->SetPosition( DoConvert( Point2i{ p_cell.m_x, p_cell.m_y } ) + Point3r{ 0, m_cellDimensions[1] / 2, 0 } );
		l_node->AttachTo( m_mapNode );
		uint32_t l_index{ 0u };

		for ( auto l_submesh : *l_geometry->GetMesh() )
		{
			l_geometry->SetMaterial( *l_submesh, m_mapCubeMaterial, false );
		}

		p_cell.m_entity = m_ecs.CreateMapBlock( l_geometry );
		m_lastMapCube = l_geometry;
		p_cell.m_state = GridCell::State::eEmpty;
	}

	void Game::DoAddTarget( GridCell & p_cell )
	{
		m_targetNode->SetPosition( DoConvert( Point2i{ p_cell.m_x, p_cell.m_y + 1 } ) );
		p_cell.m_state = GridCell::State::eTarget;
	}

	Castor3D::GeometrySPtr Game::DoCreateBullet( Entity p_source )
	{
		auto l_origin = Game::GetTowerNode( m_ecs.GetComponentData< Castor3D::GeometrySPtr >( p_source
			, m_ecs.GetComponent( Ecs::GeometryComponent ) ).GetValue() );
		String l_name = cuT( "Bullet_" ) + ToString( m_bulletSpawner.GetBulletsCount() );
		auto l_node = m_scene.GetSceneNodeCache().Add( l_name );
		auto l_geometry = m_scene.GetGeometryCache().Add( l_name, l_node, m_bulletMesh );
		l_node->SetPosition( l_origin->GetDerivedPosition() );
		l_node->AttachTo( m_mapNode );

		for ( auto l_submesh : *l_geometry->GetMesh() )
		{
			l_geometry->SetMaterial( *l_submesh, m_bulletMaterial );
		}

		return l_geometry;
	}

	GeometrySPtr Game::DoCreateTower( String const & p_name
		, GridCell & p_cell
		, Castor3D::MeshSPtr p_mesh
		, StringArray p_matNames )
	{
		auto l_node = m_scene.GetSceneNodeCache().Add( p_name );
		l_node->SetPosition( DoConvert( Point2i{ p_cell.m_x, p_cell.m_y } ) + Point3r{ 0, m_cellDimensions[1], 0 } );
		l_node->AttachTo( m_mapNode );
		auto l_tower = m_scene.GetGeometryCache().Add( p_name, l_node, p_mesh );
		l_node->SetScale( Point3r{ 0.15, 0.15, 0.15 } );
		uint32_t l_index = 0u;

		for ( auto l_name : p_matNames )
		{
			l_tower->SetMaterial( *p_mesh->GetSubmesh( l_index ), m_scene.GetMaterialView().Find( l_name ) );
			++l_index;
		}

		p_cell.m_state = GridCell::State::eTower;
		return l_tower;
	}

	 AnimatedObjectGroupSPtr Game::DoCreateAnimation( GeometrySPtr p_geometry
		, String const & p_animName )
	{
		 REQUIRE( p_geometry );
		 auto l_animGroup = m_scene.GetAnimatedObjectGroupCache().Add( p_geometry->GetName() );
		 std::chrono::milliseconds l_time{ 0 };
		 auto l_mesh = p_geometry->GetMesh();
		 REQUIRE( l_mesh );

		 if ( !p_geometry->GetAnimations().empty() )
		 {
			 auto l_object = l_animGroup->AddObject( *p_geometry, p_geometry->GetName() + cuT( "_Movable" ) );
		 }

		 if ( !l_mesh->GetAnimations().empty() )
		 {
			 auto l_object = l_animGroup->AddObject( *l_mesh, p_geometry->GetName() + cuT( "_Mesh" ) );
			 l_time = std::max( l_time
				 , l_mesh->GetAnimation( p_animName ).GetLength() );
		 }

		 auto l_skeleton = l_mesh->GetSkeleton();

		 if ( l_skeleton )
		 {
			 if ( !l_skeleton->GetAnimations().empty() )
			 {
				 auto l_object = l_animGroup->AddObject( *l_skeleton, p_geometry->GetName() + cuT( "_Skeleton" ) );
				 l_time = std::max( l_time
					 , l_skeleton->GetAnimation( p_animName ).GetLength() );
			 }
		 }

		 l_animGroup->AddAnimation( p_animName );
		 l_animGroup->SetAnimationLooped( p_animName, false );
		 l_animGroup->StartAnimation( p_animName );
		 l_animGroup->PauseAnimation( p_animName );
		 return l_animGroup;
	 }

	 void Game::DoSelectMapBlock( Entity const & p_entity )
	 {
		GeometrySPtr l_geometry = DoGetGeometry( p_entity );

		if ( l_geometry )
		{
			m_hud.UpdateTowerInfo( m_ecs, Entity{} );
			m_hud.ShowBuild();
			m_scene.GetEngine()->PostEvent( MakeFunctorEvent( EventType::ePostRender
				, [this, l_geometry, p_entity]()
				{
					m_selectedBlock.Select( p_entity, l_geometry );
					m_selectedEntity.Unselect();
				} ) );
		}
	}

	 void Game::DoSelectTower( Entity const & p_entity )
	{
		GeometrySPtr l_geometry = DoGetGeometry( p_entity );

		if ( l_geometry )
		{
			m_hud.UpdateTowerInfo( m_ecs, p_entity );
			m_hud.HideBuild();
			m_scene.GetEngine()->PostEvent( MakeFunctorEvent( EventType::ePostRender
				, [this, l_geometry, p_entity]()
				{
					m_selectedBlock.Unselect();
					m_selectedEntity.Select( p_entity, l_geometry );
				} ) );
		}
	}

	GeometrySPtr Game::DoGetGeometry( Entity const & p_entity )
	{
		GeometrySPtr l_result;

		try
		{
			l_result = m_ecs.GetComponentData< GeometrySPtr >( p_entity
				, m_ecs.GetComponent( Ecs::GeometryComponent ) ).GetValue();
		}
		catch ( Ecs::ComponentDataMatchException & p_exc )
		{
			Castor::Logger::LogWarning( p_exc.what() );
		}

		return l_result;
	}

	GridCell & Game::DoFindCell( Entity const & p_entity )
	{
		static GridCell l_invalidCell{ 0u, 0u, GridCell::State::eInvalid };
		auto l_it = std::find_if( std::begin( m_grid )
			, std::end( m_grid )
			, [&p_entity]( GridCell const & p_cell )
			{
				return p_cell.m_entity == p_entity;
			} );
		GridCell * l_result{ nullptr };

		if ( l_it != std::end( m_grid ) )
		{
			return *l_it;
		}

		return l_invalidCell;
	}

	bool Game::DoSelectEntity( Entity const & p_entity )
	{
		bool l_result = false;

		if ( m_ecs.HasComponent( p_entity, m_ecs.GetComponent( Ecs::DamageComponent ) ) )
		{
			l_result = true;
			DoSelectTower( p_entity );
		}
		else
		{
			DoSelectMapBlock( p_entity );
		}

		return l_result;
	}

	Castor::Point3r Game::DoConvert( Castor::Point2i const & p_position )const
	{
		return Castor::Point3r( ( p_position[0] - int( m_grid.GetWidth() ) / 2 ) * m_cellDimensions[0]
			, 0
			, ( p_position[1] - int( m_grid.GetHeight() ) / 2 ) * m_cellDimensions[2] );
	}
}
