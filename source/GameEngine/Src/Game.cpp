#include "Game.hpp"

#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Tower.hpp"
#include "SplashTower.hpp"

#include <Event/Frame/FrameListener.hpp>
#include <Event/Frame/FunctorEvent.hpp>
#include <Mesh/Mesh.hpp>
#include <Mesh/Submesh.hpp>
#include <Scene/Geometry.hpp>
#include <Scene/Scene.hpp>

using namespace Castor;
using namespace Castor3D;

namespace orastus
{
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
	{
		m_mapNode = m_scene.GetSceneNodeCache().Find( cuT( "MapBase" ) );
		m_mapCubeMesh = m_scene.GetMeshCache().Find( cuT( "MapCube" ) );
		m_mapCubeMaterial = m_scene.GetMaterialView().Find( cuT( "MapCube" ) );
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
		m_selectedTower = nullptr;
		m_state = State::eStarted;
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
		DoAddTarget( GetCell( l_node.m_x, l_node.m_y ) );

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
			m_ecs.Update();
			m_saved = Clock::now();
		}
	}

	GridCell & Game::GetCell( int p_x, int p_y )
	{
		return m_grid( p_y, p_x );
	}

	GridCell & Game::GetCell( Castor::Point2i const & p_position )
	{
		return GetCell( p_position[0], p_position[1] );
	}

	GridCell & Game::GetCell( Castor::Point3r const & p_position )
	{
		static GridCell l_dummy;
		l_dummy.m_state = GridCell::State::eInvalid;
		auto l_coords = Convert( p_position );

		if ( l_coords[0] >= 0 && l_coords[0] < int( m_grid.GetWidth() )
			&& l_coords[1] >= 0 && l_coords[1] < int( m_grid.GetHeight() ) )
		{
			return GetCell( l_coords );
		}

		return l_dummy;
	}

	GridCell const & Game::GetCell( int p_x, int p_y )const
	{
		return m_grid( p_y, p_x );
	}

	GridCell const & Game::GetCell( Castor::Point2i const & p_position )const
	{
		return GetCell( p_position[0], p_position[1] );
	}

	GridCell const & Game::GetCell( Castor::Point3r const & p_position )const
	{
		static GridCell l_dummy;
		l_dummy.m_state = GridCell::State::eInvalid;
		auto l_coords = Convert( p_position );
		GridCell & l_return = l_dummy;

		if ( l_coords[0] >= 0 && l_coords[0] < int( m_grid.GetWidth() )
			&& l_coords[1] >= 0 && l_coords[1] < int( m_grid.GetHeight() ) )
		{
			l_return = GetCell( l_coords );
		}

		return l_dummy;
	}

	Castor::Point3r Game::Convert( Castor::Point2i const & p_position )const
	{
		return Castor::Point3r( ( p_position[0] - int( m_grid.GetWidth() ) / 2 ) * m_cellDimensions[0]
			, 0
			, ( p_position[1] - int( m_grid.GetHeight() ) / 2 ) * m_cellDimensions[2] );
	}

	Castor::Point2i Game::Convert( Castor::Point3r const & p_position )const
	{
		return Castor::Point2i( p_position[0] / m_cellDimensions[0] + m_grid.GetWidth() / 2
			, p_position[2] / m_cellDimensions[2] + m_grid.GetHeight() / 2 );
	}

	Entity * Game::SelectTower( GridCell const & p_cell )
	{
		Entity * l_return;

		if ( p_cell.m_state == GridCell::State::eTower )
		{
			auto l_it = std::find_if( m_grid.begin()
				, m_grid.end()
				, [&p_cell]( GridCell const & p_lookup )
			{
				return p_lookup.m_x == p_cell.m_x
					&& p_lookup.m_y == p_cell.m_y
					&& p_lookup.m_entity;
			} );

			if ( l_it != m_grid.end() )
			{
				l_return = l_it->m_entity;
				m_selectedTower = l_return;
			}
		}

		return l_return;
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
					GetCell( l_prv->m_x, l_prv->m_y ).m_state = GridCell::State::eStart;
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
		l_node->SetPosition( Convert( Point2i{ p_cell.m_x, p_cell.m_y } ) + Point3r{ 0, m_cellDimensions[1] / 2, 0 } );
		l_node->AttachTo( m_mapNode );
		uint32_t l_index{ 0u };

		for ( auto l_submesh : *l_geometry->GetMesh() )
		{
			l_geometry->SetMaterial( *l_submesh, m_mapCubeMaterial, false );
		}

		m_lastMapCube = l_geometry;
		p_cell.m_state = GridCell::State::eEmpty;
	}

	void Game::DoAddTarget( GridCell & p_cell )
	{
		m_targetNode->SetPosition( Convert( Point2i{ p_cell.m_x, p_cell.m_y + 1 } ) );
		p_cell.m_state = GridCell::State::eTarget;
	}
}
