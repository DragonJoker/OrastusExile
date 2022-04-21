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
#include "TargetSpawner.hpp"
#include "ECS/Ecs.hpp"
#include "ECS/SoundSource.hpp"
#include "ECS/TowerCategory.hpp"

#include <Castor3D/Scene/Camera.hpp>

#include <random>

namespace orastus
{
	castor3d::AnimatedObjectGroupSPtr createAnimation( castor3d::GeometrySPtr geometry
		, String const & animName
		, bool looped = false
		, bool paused = true );
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
		EFO_API explicit Game( castor::Path const & dataFolder
			, castor3d::Scene & scene
			, castor3d::Camera const & camera );
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
		*	An enemy is capturing a target.
		*\param[in] entity
		*	The enemy entity.
		*/
		EFO_API void enemyCapturing( EnemyData const & enemy );
		/**
		*\brief
		*	An enemy is escaping with its target.
		*\param[in] entity
		*	The enemy entity.
		*/
		EFO_API void enemyEscaping( EnemyData const & enemy );
		/**
		*\brief
		*	An enemy has escaped.
		*\param[in] entity
		*	The enemy entity.
		*/
		EFO_API void enemyEscaped( EnemyData const & enemy );
		/**
		*\brief
		*	Kills a bullet.
		*\param[in] bullet
		*	The bullet.
		*/
		EFO_API void killBullet( BulletData const & bullet );
		/**
		*\brief
		*	Hits a target.
		*\param[in] bullet
		*	The bullet.
		*\param[in] target
		*	The hit target.
		*\param[in] damage
		*	The hit damage.
		*/
		EFO_API void hit( BulletData const & bullet
			, Entity target
			, uint32_t damage );
		/**
		*\brief
		*	Area damage around a target.
		*\param[in] bullet
		*	The bullet.
		*\param[in] target
		*	The hit target.
		*\param[in] damage
		*	The hit damage.
		*/
		EFO_API void areaDamage( BulletData const & bullet
			, Entity target
			, uint32_t damage );
		/**
		*\brief
		*	Select an available target to capture.
		*\return
		*	The target entity.
		*/
		EFO_API Entity selectTarget();
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
		*	Creates a direct tower.
		*/
		EFO_API void createDirectTower();
		/**
		*\brief
		*	Creates a splash tower.
		*/
		EFO_API void createSplashTower();
		/**
		*\brief
		*	Creates a bullet.
		*\param[in] source
		*	The firing entity.
		*\param[in] target
		*	The target entity.
		*/
		EFO_API void createBullet( TowerData const & source
			, Entity target );
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
		*\return
		*	The sound emitted when a target is captured.
		*/
		EFO_API Sound & getTargetCapturedSound();
		/**
		*\brief
		*	Creates an SFX sound.
		*/
		EFO_API Sound & addSound( castor::Path const & file
			, uint32_t maxSources );
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
		*\brief
		*	Retrieves the scene node for given target's geometry.
		*\param[in] geometry
		*	The geometry.
		*\return
		*	The scene node.
		*/
		EFO_API static castor3d::SceneNodeRPtr getTargetNode( castor3d::GeometrySPtr geometry );
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
		castor3d::Scene & getScene()const
		{
			return m_scene;
		}
		/**
		*\return
		*	The scene.
		*/
		castor3d::SceneNode const & getCameraNode()const
		{
			return *m_camera.getParent();
		}
		/**
		*\return
		*	The map base node.
		*/
		castor3d::SceneNode & getMapNode()const
		{
			return *m_mapNode;
		}
		/**
		*\return
		*	The current life count.
		*/
		uint32_t getLives()const
		{
			return m_targetSpawner.getLives();
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
		/**
		*\return
		*	The current wave enemies bounty.
		*/
		uint32_t getEnemiesBounty()const
		{
			return m_enemySpawner.getEnemiesBounty();
		}
		/**
		*\return
		*	The random engine.
		*/
		std::default_random_engine & getRandomEngine()
		{
			return m_engine;
		}
		/**
		*\return
		*	The wave start sound.
		*/
		SoundSource const & getWaveStartSound()const
		{
			return m_waveStartSound;
		}
		/**
		*\return
		*	The enemy kill sound.
		*/
		Sound & getEnemyKillSound()const
		{
			return m_enemyKillSound;
		}
		/**
		*\return
		*	The audio system.
		*/
		Audio const & getAudio()const
		{
			return *m_audio;
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
		void doAddTargets( GridCell & cell );
		castor3d::MeshResPtr doGetAmmoMesh( AmmoType type );
		castor3d::GeometrySPtr doCreateAmmo( castor3d::SceneNode const & origin
			, AmmoType type );
		castor3d::GeometrySPtr doCreateTowerBase( castor::String const & name
			, GridCell & cell
			, castor3d::MeshResPtr mesh );
		castor3d::GeometrySPtr doCreateTower( castor::String const & name
			, GridCell & cell
			, castor3d::Mesh const & base
			, castor3d::MeshResPtr mesh );
		void doSelectMapBlock( Entity const & entity );
		void doSelectTower( Entity const & entity );
		castor3d::GeometrySPtr doGetGeometry( Entity const & entity );
		GridCell & doFindCell( Entity const & entity );
		bool doSelectEntity( Entity const & entity );
		castor::Point3f doConvert( castor::Point2i const & position )const;

	private:
		// Persistent data.
		AudioUPtr m_audio;
		Ecs m_ecs;
		castor3d::Scene & m_scene;
		castor3d::Camera const & m_camera;
		Grid m_grid;
		Hud m_hud;
		GridPath m_path;
		castor3d::SceneNodeSPtr m_mapNode;
		castor3d::MeshResPtr m_emptyTileMesh;
		castor::Point3f m_cellDimensions;
		castor3d::MeshResPtr m_pathStartTileMesh;
		castor3d::MeshResPtr m_pathStraightTileMesh;
		castor3d::MeshResPtr m_pathTurnTileMesh;
		castor3d::MeshResPtr m_pathAreaTileMesh;
		castor3d::MeshResPtr m_towerBaseMesh;
		castor3d::MeshResPtr m_directTowerMesh;
		castor3d::MeshResPtr m_splashTowerMesh;
		castor3d::MeshResPtr m_arrowMesh;
		castor3d::MeshResPtr m_cannonBallMesh;
		std::vector< Sound * > m_targetCapturedSounds;
		Sound & m_ballistaShootSound;
		Sound & m_cannonShootSound;
		Sound & m_ballistaHitSound;
		Sound & m_cannonHitSound;
		Sound & m_enemyKillSound;
		SoundSource const & m_towerBuildSound;
		SoundSource const & m_waveStartSound;
		std::random_device m_device;
		std::default_random_engine m_engine;
		// Varying data.
		GridCell * m_selectedCell{ nullptr };
		Clock::time_point m_saved;
		std::chrono::milliseconds m_elapsed;
		State m_state;
		SelectedEntity m_selectedEntity;
		SelectedEntity m_selectedBlock;
		EnemySpawner m_enemySpawner;
		BulletSpawner m_bulletSpawner;
		TargetSpawner m_targetSpawner;
	};
}

#endif
