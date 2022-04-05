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
			void select( Entity const & p_entity
				, castor3d::GeometrySPtr p_geometry );
			void unselect();

			Entity m_entity;
			castor3d::GeometrySPtr m_geometry;
			std::vector< castor3d::MaterialRPtr > m_materials;
		};

		struct SelectedBlock
		{
			void select( Entity const & p_entity
				, castor3d::GeometrySPtr p_geometry );
			void unselect();

			Entity m_entity;
			castor3d::GeometrySPtr m_geometry;
			castor3d::MaterialRPtr m_material;
		};

	public:
		/**
		*\brief
		*	Constructor, initialises the base components.
		*\param[in] p_scene
		*	The 3D scene.
		*/
		EFO_API explicit Game( castor3d::Scene & p_scene );
		/**
		*\brief
		*	Resets the game so a new game can be played.
		*/
		EFO_API void reset();
		/**
		*\brief
		*	Starts the game.
		*/
		EFO_API void start();
		/**
		*\brief
		*	Pauses the game.
		*/
		EFO_API void pause();
		/**
		*\brief
		*	Resumes to the game from a pause.
		*/
		EFO_API void resume();
		/**
		*\brief
		*	Displays help.
		*/
		EFO_API void help();
		/**
		*\brief
		*	Game loop function.
		*/
		EFO_API void update();
		/**
		*\brief
		*	Selects the cell for given geometry.
		*\param[in] p_geometry
		*	The geometry.
		*\return
		*	The tower entity at selected cell.
		*/
		EFO_API Entity select( castor3d::GeometrySPtr p_geometry );
		/**@}*/
		/**
		*\brief
		*	Unselects the current selection.
		*/
		EFO_API void unselect();
		/**
		*\brief
		*	Kills an enemy.
		*\param[in] p_entity
		*	The enemy entity.
		*/
		EFO_API void killEnemy( Entity p_entity );
		/**
		*\brief
		*	Kills a bullet.
		*\param[in] p_entity
		*	The bullet entity.
		*/
		EFO_API void killBullet( Entity p_entity );
		/**
		*\brief
		*	Hits a target.
		*\param[in] p_source
		*	The hit source.
		*\param[in] p_target
		*	The hit target.
		*/
		EFO_API void hit( Entity p_source
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
		castor3d::GeometrySPtr createEnemy( castor::String const & p_name );
		/**
		*\brief
		*	Creates a short range tower.
		*/
		EFO_API void createShortRangeTower();
		/**
		*\brief
		*	Creates a long range tower.
		*/
		EFO_API void createLongRangeTower();
		/**
		*\brief
		*	Creates a bullet.
		*\param[in] p_source
		*	The firing entity.
		*\param[in] p_target
		*	The target entity.
		*/
		EFO_API void createBullet( Entity p_source, Entity p_target );
		/**
		*\brief
		*	Converts a cell position to 3D position.
		*\param[in] p_position
		*	The cell position.
		*\return
		*	The 3D position
		*/
		EFO_API castor::Point3f convert( castor::Point2i const & p_position )const;
		/**
		*\brief
		*	Retrieves the scene node for given enemy's geometry.
		*\param[in] p_geometry
		*	The geometry.
		*\return
		*	The scene node.
		*/
		EFO_API static castor3d::SceneNodeRPtr getEnemyNode( castor3d::GeometrySPtr p_geometry );
		/**
		*\brief
		*	Retrieves the scene node for given tower's geometry.
		*\param[in] p_geometry
		*	The geometry.
		*\return
		*	The scene node.
		*/
		EFO_API static castor3d::SceneNodeRPtr getTowerNode( castor3d::GeometrySPtr p_geometry );
		/**
		*\brief
		*	Retrieves the scene node for given bullet's geometry.
		*\param[in] p_geometry
		*	The geometry.
		*\return
		*	The scene node.
		*/
		EFO_API static castor3d::SceneNodeRPtr getBulletNode( castor3d::GeometrySPtr p_geometry );
		/**
		*\return
		*	\p true if the game is started.
		*/
		inline bool isStarted()const
		{
			return m_state >= State::eStarted && m_state < State::eEnded;
		}
		/**
		*\return
		*	\p true if the game is started and running.
		*/
		inline bool isRunning()const
		{
			return m_state == State::eStarted;
		}
		/**
		*\return
		*	\p true if the game is ended.
		*/
		inline bool isEnded()const
		{
			return m_state == State::eEnded;
		}
		/**
		*\return
		*	\p true if the game is paused.
		*/
		inline bool isPaused()const
		{
			return m_state == State::ePaused;
		}
		/**
		*\return
		*	The cell height.
		*/
		inline float getCellHeight()const
		{
			return m_cellDimensions[2];
		}
		/**
		*\return
		*	The current enemy path.
		*/
		inline GridPath const & getPath()const
		{
			return m_path;
		}
		/**
		*\return
		*	The current level grid.
		*/
		inline Grid const & getGrid()const
		{
			return m_grid;
		}

	private:
		void doPrepareGrid();
		void doAddMapCube( GridCell & p_cell );
		void doAddTarget( GridCell & p_cell );
		castor3d::GeometrySPtr doCreateBullet( Entity p_source );
		castor3d::GeometrySPtr doCreateTower( castor::String const & p_name
			, GridCell & p_cell
			, castor3d::MeshResPtr p_mesh
			, castor::StringArray p_matNames );
		castor3d::AnimatedObjectGroupSPtr doCreateAnimation( castor3d::GeometrySPtr p_geometry
			, castor::String const & p_animName );
		void doSelectMapBlock( Entity const & p_entity );
		void doSelectTower( Entity const & p_entity );
		castor3d::GeometrySPtr doGetGeometry( Entity const & p_entity );
		GridCell & doFindCell( Entity const & p_entity );
		bool doSelectEntity( Entity const & p_entity );
		castor::Point3f doConvert( castor::Point2i const & p_position )const;

	private:
		// Persistent data.
		Ecs m_ecs;
		castor3d::Scene & m_scene;
		Grid m_grid;
		Hud m_hud;
		GridPath m_path;
		castor::Point3f m_cellDimensions;
		castor3d::SceneNodeSPtr m_mapNode;
		castor3d::SceneNodeSPtr m_targetNode;
		castor3d::MeshResPtr m_mapCubeMesh;
		castor3d::MaterialSPtr m_mapCubeMaterial;
		castor3d::MeshResPtr m_shortRangeTowerMesh;
		castor3d::MeshResPtr m_longRangeTowerMesh;
		castor3d::MeshResPtr m_enemyCubeMesh;
		castor3d::MaterialSPtr m_enemyCubeMaterial;
		castor3d::MeshResPtr m_bulletMesh;
		castor3d::MaterialSPtr m_bulletMaterial;
		// Varying data.
		GridCell * m_selectedCell{ nullptr };
		Clock::time_point m_saved;
		std::chrono::milliseconds m_elapsed;
		State m_state;
		castor3d::GeometrySPtr m_lastMapCube;
		SelectedEntity m_selectedEntity;
		SelectedBlock m_selectedBlock;
		EnemySpawner m_enemySpawner;
		BulletSpawner m_bulletSpawner;
	};
}

#endif
