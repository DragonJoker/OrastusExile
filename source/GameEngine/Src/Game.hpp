/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Game_HPP___
#define ___EFO_Game_HPP___

#include "Ecs.hpp"
#include "Grid.hpp"
#include "Hud.hpp"

namespace orastus
{
	/**
	*\brief
	*	Main class for the game logic.
	*/
	class Game
	{
	private:
		enum class State
		{
			eInitial,
			eStarted,
			ePaused,
			eEnded,
		};

	public:
		/**
		*\brief
		*	Constructor, initialises the base components.
		*\param[in] p_scene
		*	The 3D scene.
		*/
		EFO_API Game( Castor3D::Scene & p_scene );
		/**
		*\brief
		*	Resets the game so a new game can be played.
		*/
		EFO_API void Reset();
		/**
		*\brief
		*	Starts the game.
		*/
		EFO_API void Start();
		/**
		*\brief
		*	Pauses the game.
		*/
		EFO_API void Pause();
		/**
		*\brief
		*	Resumes to the game from a pause.
		*/
		EFO_API void Resume();
		/**
		*\brief
		*	Displays help.
		*/
		EFO_API void Help();
		/**
		*\brief
		*	Game loop function.
		*/
		EFO_API void Update();
		/**
		*\name GridCell retrieval.
		*/
		/**@{*/
		//! From a 3D float position.
		EFO_API GridCell & GetCell( Castor::Point3r const & p_position );
		//! From a 2D int position.
		EFO_API GridCell & GetCell( Castor::Point2i const & p_position );
		//! From a 2D int position.
		EFO_API GridCell & GetCell( int p_x, int p_y );
		//! From a 3D float position.
		EFO_API GridCell const & GetCell( Castor::Point3r const & p_position )const;
		//! From a 2D int position.
		EFO_API GridCell const & GetCell( Castor::Point2i const & p_position )const;
		//! From a 2D int position.
		EFO_API GridCell const & GetCell( int p_x, int p_y )const;
		/**
		*\brief
		*	Converts a 2D grid cell position to a 3D in-scene position.
		*\param[in] p_position
		*	The cell position.
		*\return
		*	The in-scene position.
		*/
		EFO_API Castor::Point3r Convert( Castor::Point2i const & p_position )const;
		/**
		*\brief
		*	Converts a 3D in-scene position to a 2D grid cell position.
		*\param[in] p_position
		*	The in-scene position.
		*\return
		*	The cell position.
		*/
		EFO_API Castor::Point2i Convert( Castor::Point3r const & p_position )const;
		/**@}*/
		/**
		*\brief
		*	Selects the tower at given cell.
		*\param[in] p_cell
		*	The cell.
		*\return
		*	The cell position.
		*/
		EFO_API Entity * SelectTower( GridCell const & p_cell );
		/**
		*\return
		*	\p true if the game is started.
		*/
		inline bool IsStarted()const
		{
			return m_state >= State::eStarted && m_state < State::eEnded;
		}
		/**@}*/
		/**
		*\return
		*	\p true if the game is started and running.
		*/
		inline bool IsRunning()const
		{
			return m_state == State::eStarted;
		}
		/**@}*/
		/**
		*\return
		*	\p true if the game is ended.
		*/
		inline bool IsEnded()const
		{
			return m_state == State::eEnded;
		}
		/**@}*/
		/**
		*\return
		*	\p true if the game is paused.
		*/
		inline bool IsPaused()const
		{
			return m_state == State::ePaused;
		}

	private:
		void DoPrepareGrid();
		void DoAddMapCube( GridCell & p_cell );
		void DoAddTarget( GridCell & p_cell );

	private:
		// Persistent data.
		Ecs m_ecs;
		Castor3D::Scene & m_scene;
		Grid m_grid;
		Hud m_hud;
		GridPath m_path;
		Castor::Point3r m_cellDimensions;
		Castor3D::SceneNodeSPtr m_mapNode;
		Castor3D::SceneNodeSPtr m_targetNode;
		Castor3D::MeshSPtr m_mapCubeMesh;
		Castor3D::MaterialSPtr m_mapCubeMaterial;
		Castor3D::MeshSPtr m_shortRangeTowerMesh;
		Castor3D::MeshSPtr m_longRangeTowerMesh;
		Castor3D::MeshSPtr m_enemyCubeMesh;
		Castor3D::MaterialSPtr m_enemyCubeMaterial;
		Castor3D::MeshSPtr m_bulletMesh;
		Castor3D::MaterialSPtr m_bulletMaterial;
		// Varying data.
		Entity * m_selectedTower{ nullptr };
		Clock::time_point m_saved;
		std::chrono::milliseconds m_elapsed;
		State m_state;
		Castor3D::GeometrySPtr m_lastMapCube;
	};
}

#endif
