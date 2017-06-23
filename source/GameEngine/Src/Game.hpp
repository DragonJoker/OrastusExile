/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Game_HPP___
#define ___EFO_Game_HPP___

#include "BulletSpawner.hpp"
#include "EnemySpawner.hpp"
#include "Grid.hpp"
#include "Hud.hpp"
#include "ECS/Ecs.hpp"

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

		struct SelectedEntity
		{
			void Select( Entity const & p_entity
				, Castor3D::GeometrySPtr p_geometry );
			void Unselect();

			Entity m_entity;
			Castor3D::GeometrySPtr m_geometry;
			std::vector< Castor3D::MaterialSPtr > m_materials;
		};

		struct SelectedBlock
		{
			void Select( Entity const & p_entity
				, Castor3D::GeometrySPtr p_geometry );
			void Unselect();

			Entity m_entity;
			Castor3D::GeometrySPtr m_geometry;
			Castor3D::MaterialSPtr m_material;
		};

	public:
		/**
		*\brief
		*	Constructor, initialises the base components.
		*\param[in] p_scene
		*	The 3D scene.
		*/
		EFO_API explicit Game( Castor3D::Scene & p_scene );
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
		*\brief
		*	Selects the cell for given geometry.
		*\param[in] p_geometry
		*	The geometry.
		*\return
		*	The tower entity at selected cell.
		*/
		EFO_API Entity Select( Castor3D::GeometrySPtr p_geometry );
		/**@}*/
		/**
		*\brief
		*	Unselects the current selection.
		*/
		EFO_API void Unselect();
		/**
		*\brief
		*	Kills an enemy.
		*\param[in] p_entity
		*	The enemy entity.
		*/
		EFO_API void KillEnemy( Entity p_entity );
		/**
		*\brief
		*	Hits a target.
		*\param[in] p_source
		*	The hit source.
		*\param[in] p_target
		*	The hit target.
		*/
		EFO_API void Hit( Entity p_source
			, Entity p_target
			, uint32_t p_damage );
		/**
		*\brief
		*	Creates an enemy geometry.
		*\param[in] p_name
		*	The name.
		*\return
		*	The created geometry.
		*/
		Castor3D::GeometrySPtr CreateEnemy( Castor::String const & p_name );
		/**
		*\brief
		*	Creates a short range tower.
		*/
		EFO_API void CreateShortRangeTower();
		/**
		*\brief
		*	Creates a long range tower.
		*/
		EFO_API void CreateLongRangeTower();
		/**
		*\brief
		*	Creates a bullet.
		*\param[in] p_source
		*	The firing entity.
		*\param[in] p_target
		*	The target entity.
		*/
		EFO_API void CreateBullet( Entity p_source, Entity p_target );
		/**
		*\brief
		*	Converts a cell position to 3D position.
		*\param[in] p_position
		*	The cell position.
		*\return
		*	The 3D position
		*/
		EFO_API Castor::Point3r Convert( Castor::Point2i const & p_position )const;
		/**
		*\brief
		*	Retrieves the scene node for given enemy's geometry.
		*\param[in] p_geometry
		*	The geometry.
		*\return
		*	The scene node.
		*/
		EFO_API static Castor3D::SceneNodeSPtr GetEnemyNode( Castor3D::GeometrySPtr p_geometry );
		/**
		*\brief
		*	Retrieves the scene node for given tower's geometry.
		*\param[in] p_geometry
		*	The geometry.
		*\return
		*	The scene node.
		*/
		EFO_API static Castor3D::SceneNodeSPtr GetTowerNode( Castor3D::GeometrySPtr p_geometry );
		/**
		*\brief
		*	Retrieves the scene node for given bullet's geometry.
		*\param[in] p_geometry
		*	The geometry.
		*\return
		*	The scene node.
		*/
		EFO_API static Castor3D::SceneNodeSPtr GetBulletNode( Castor3D::GeometrySPtr p_geometry );
		/**
		*\return
		*	\p true if the game is started.
		*/
		inline bool IsStarted()const
		{
			return m_state >= State::eStarted && m_state < State::eEnded;
		}
		/**
		*\return
		*	\p true if the game is started and running.
		*/
		inline bool IsRunning()const
		{
			return m_state == State::eStarted;
		}
		/**
		*\return
		*	\p true if the game is ended.
		*/
		inline bool IsEnded()const
		{
			return m_state == State::eEnded;
		}
		/**
		*\return
		*	\p true if the game is paused.
		*/
		inline bool IsPaused()const
		{
			return m_state == State::ePaused;
		}
		/**
		*\return
		*	The cell height.
		*/
		inline float GetCellHeight()const
		{
			return m_cellDimensions[2];
		}
		/**
		*\return
		*	The current enemy path.
		*/
		inline GridPath const & GetPath()const
		{
			return m_path;
		}
		/**
		*\return
		*	The current level grid.
		*/
		inline Grid const & GetGrid()const
		{
			return m_grid;
		}

	private:
		void DoPrepareGrid();
		void DoAddMapCube( GridCell & p_cell );
		void DoAddTarget( GridCell & p_cell );
		Castor3D::GeometrySPtr DoCreateBullet( Entity p_source );
		Castor3D::GeometrySPtr DoCreateTower( Castor::String const & p_name
			, GridCell & p_cell
			, Castor3D::MeshSPtr p_mesh
			, Castor::StringArray p_matNames );
		Castor3D::AnimatedObjectGroupSPtr DoCreateAnimation( Castor3D::GeometrySPtr p_geometry
			, Castor::String const & p_animName );
		void DoSelectMapBlock( Entity const & p_entity );
		void DoSelectTower( Entity const & p_entity );
		Castor3D::GeometrySPtr DoGetGeometry( Entity const & p_entity );
		GridCell & DoFindCell( Entity const & p_entity );
		bool DoSelectEntity( Entity const & p_entity );
		Castor::Point3r DoConvert( Castor::Point2i const & p_position )const;

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
		GridCell * m_selectedCell{ nullptr };
		Clock::time_point m_saved;
		std::chrono::milliseconds m_elapsed;
		State m_state;
		Castor3D::GeometrySPtr m_lastMapCube;
		SelectedEntity m_selectedEntity;
		SelectedBlock m_selectedBlock;
		EnemySpawner m_enemySpawner;
		BulletSpawner m_bulletSpawner;
	};
}

#endif
