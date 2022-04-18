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
			void select( Entity const & entity
				, castor3d::GeometrySPtr geometry );
			void unselect();

			Entity entity;
			castor3d::GeometrySPtr geometry;
			std::vector< castor3d::MaterialRPtr > materials;
			std::vector< castor3d::MaterialRPtr > selMaterials;
		};

	public:
		/**
		*\brief
		*	Constructor, initialises the base components.
		*\param[in] scene
		*	The 3D scene.
		*/
		EFO_API explicit Game( castor3d::Scene & scene );
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
		*\param[in] geometry
		*	The geometry.
		*\return
		*	The tower entity at selected cell.
		*/
		EFO_API Entity select( castor3d::GeometrySPtr geometry );
		/**@}*/
		/**
		*\brief
		*	Unselects the current selection.
		*/
		EFO_API void unselect();
		/**
		*\brief
		*	Kills an enemy.
		*\param[in] entity
		*	The enemy entity.
		*/
		EFO_API void killEnemy( Entity entity );
		/**
		*\brief
		*	An enemy has arrived to the destination.
		*\param[in] entity
		*	The enemy entity.
		*/
		EFO_API void enemyArrived( Entity entity );
		/**
		*\brief
		*	Kills a bullet.
		*\param[in] entity
		*	The bullet entity.
		*/
		EFO_API void killBullet( Entity entity );
		/**
		*\brief
		*	Hits a target.
		*\param[in] source
		*	The hit source.
		*\param[in] target
		*	The hit target.
		*\param[in] damage
		*	The hit damage.
		*/
		EFO_API void hit( Entity source
			, Entity target
			, uint32_t damage );
		/**
		*\brief
		*	Makes the player lose a life.
		*/
		EFO_API void loseLife();
		/**
		*\brief
		*	Creates an enemy geometry.
		*\param[in] name
		*	The name.
		*\return
		*	The created geometry.
		*/
		EFO_API castor3d::GeometrySPtr createEnemy( castor::String const & name
			, castor3d::MeshResPtr mesh );
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
		*\param[in] source
		*	The firing entity.
		*\param[in] target
		*	The target entity.
		*/
		EFO_API void createBullet( Entity source, Entity target );
		/**
		*\brief
		*	Converts a cell position to 3D position.
		*\param[in] position
		*	The cell position.
		*\return
		*	The 3D position
		*/
		EFO_API castor::Point3f convert( castor::Point2i const & position )const;
		/**
		*\brief
		*	Retrieves the scene node for given enemy's geometry.
		*\param[in] geometry
		*	The geometry.
		*\return
		*	The scene node.
		*/
		EFO_API static castor3d::SceneNodeRPtr getEnemyNode( castor3d::GeometrySPtr geometry );
		/**
		*\brief
		*	Retrieves the scene node for given tower's geometry.
		*\param[in] geometry
		*	The geometry.
		*\return
		*	The scene node.
		*/
		EFO_API static castor3d::SceneNodeRPtr getTowerNode( castor3d::GeometrySPtr geometry );
		/**
		*\brief
		*	Retrieves the scene node for given bullet's geometry.
		*\param[in] geometry
		*	The geometry.
		*\return
		*	The scene node.
		*/
		EFO_API static castor3d::SceneNodeRPtr getBulletNode( castor3d::GeometrySPtr geometry );
		/**
		*\return
		*	\p true if the game is started.
		*/
		bool isStarted()const
		{
			return m_state >= State::eStarted && m_state < State::eEnded;
		}
		/**
		*\return
		*	\p true if the game is started and running.
		*/
		bool isRunning()const
		{
			return m_state == State::eStarted;
		}
		/**
		*\return
		*	\p true if the game is ended.
		*/
		bool isEnded()const
		{
			return m_state == State::eEnded;
		}
		/**
		*\return
		*	\p true if the game is paused.
		*/
		bool isPaused()const
		{
			return m_state == State::ePaused;
		}
		/**
		*\return
		*	The cell height.
		*/
		float getCellHeight()const
		{
			return m_cellDimensions[2];
		}
		/**
		*\return
		*	The current enemy path.
		*/
		GridPath const & getPath()const
		{
			return m_path;
		}
		/**
		*\return
		*	The current level grid.
		*/
		Grid const & getGrid()const
		{
			return m_grid;
		}
		/**
		*\return
		*	The scene.
		*/
		castor3d::Scene const & getScene()const
		{
			return m_scene;
		}
		/**
		*\return
		*	The current wave.
		*/
		uint32_t getWave()const
		{
			return m_enemySpawner.getWave();
		}
		/**
		*\return
		*	The total enemy kill count.
		*/
		uint32_t getKills()const
		{
			return m_enemySpawner.getKills();
		}
		/**
		*\return
		*	The current wave enemies life points.
		*/
		uint32_t getEnemiesLife()const
		{
			return m_enemySpawner.getEnemiesLife();
		}

	private:
		void doPrepareGrid();
		void doAddTile( GridCell & cell
			, castor3d::MeshResPtr mesh
			, castor::Quaternion const & orientation
			, bool pickable );
		void doAddEmptyTile( GridCell & cell );
		void doAddPathTile( GridCell & cell
			, castor3d::MeshResPtr mesh
			, castor::Quaternion const & orientation );
		void doAddTarget( GridCell & cell );
		castor3d::GeometrySPtr doCreateBullet( Entity source );
		castor3d::GeometrySPtr doCreateTowerBase( castor::String const & name
			, GridCell & cell
			, castor3d::MeshResPtr mesh );
		castor3d::GeometrySPtr doCreateTower( castor::String const & name
			, GridCell & cell
			, castor3d::Mesh const & base
			, castor3d::MeshResPtr mesh );
		castor3d::AnimatedObjectGroupSPtr doCreateAnimation( castor3d::GeometrySPtr geometry
			, castor::String const & animName );
		void doSelectMapBlock( Entity const & entity );
		void doSelectTower( Entity const & entity );
		castor3d::GeometrySPtr doGetGeometry( Entity const & entity );
		GridCell & doFindCell( Entity const & entity );
		bool doSelectEntity( Entity const & entity );
		castor::Point3f doConvert( castor::Point2i const & position )const;

	private:
		// Persistent data.
		Ecs m_ecs;
		castor3d::Scene & m_scene;
		Grid m_grid;
		Hud m_hud;
		GridPath m_path;
		castor3d::SceneNodeSPtr m_mapNode;
		castor3d::SceneNodeSPtr m_targetNode;
		castor3d::MeshResPtr m_emptyTileMesh;
		castor::Point3f m_cellDimensions;
		castor3d::MeshResPtr m_pathStartTileMesh;
		castor3d::MeshResPtr m_pathStraightTileMesh;
		castor3d::MeshResPtr m_pathTurnTileMesh;
		castor3d::MeshResPtr m_pathAreaTileMesh;
		castor3d::MeshResPtr m_towerBaseMesh;
		castor3d::MeshResPtr m_shortRangeTowerMesh;
		castor3d::MeshResPtr m_longRangeTowerMesh;
		castor3d::MeshResPtr m_bulletMesh;
		castor3d::MaterialSPtr m_bulletMaterial;
		// Varying data.
		GridCell * m_selectedCell{ nullptr };
		Clock::time_point m_saved;
		std::chrono::milliseconds m_elapsed;
		State m_state;
		SelectedEntity m_selectedEntity;
		SelectedEntity m_selectedBlock;
		EnemySpawner m_enemySpawner;
		BulletSpawner m_bulletSpawner;
	};
}

#endif
