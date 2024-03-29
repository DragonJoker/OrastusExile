/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Hud_HPP___
#define ___EFO_Hud_HPP___

#include "GameEnginePrerequisites.hpp"
#include "Ecs/SoundSource.hpp"

namespace orastus
{
	/**
	*\brief
	*	The game HUD.
	*/
	class Hud
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] game
		*	The game instance.
		*\param[in] scene
		*	The scene.
		*/
		EFO_API Hud( Game & game
			, castor::Path const & dataFolder
			, castor3d::Scene const & scene );
		/**
		*\brief
		*	Sets the HUD to initial state.
		*/
		EFO_API void initialise();
		/**
		*\brief
		*	Shows "game running" HUD.
		*/
		EFO_API void start();
		/**
		*\brief
		*	Shows "game paused" HUD.
		*/
		EFO_API void pause();
		/**
		*\brief
		*	Shows "game running" HUD.
		*/
		EFO_API void resume();
		/**
		*\brief
		*	Shows "help" HUD.
		*/
		EFO_API void help();
		/**
		*\brief
		*	Shows "game over" HUD.
		*/
		EFO_API void gameOver();
		/**
		*\brief
		*	Shows the build tower options.
		*/
		EFO_API void showBuild();
		/**
		*\brief
		*	Shows the build tower options.
		*/
		EFO_API void hideBuild();
		/**
		*\brief
		*	Shows the upgrade tower options.
		*/
		EFO_API void showUpgrade();
		/**
		*\brief
		*	Shows the upgrade tower options.
		*/
		EFO_API void hideUpgrade();
		/**
		*\brief
		*	Updates the HUD.
		*/
		EFO_API void update();
		/**
		*\brief
		*	Updates the HUD tower info.
		*/
		EFO_API void updateTowerInfo( Ecs const & ecs
			, Entity const & entity );

	private:
		Game & m_game;
		castor3d::Scene const & m_scene;
		castor3d::OverlayWPtr m_titlePanel;
		castor3d::OverlayWPtr m_gameEndPanel;
		castor3d::OverlayWPtr m_helpPanel;
		castor3d::OverlayWPtr m_hudResources;
		castor3d::OverlayWPtr m_hudScore;
		castor3d::OverlayWPtr m_hudDetails;
		castor3d::OverlayWPtr m_hudPause;
		castor3d::OverlayWPtr m_hudBuild;
		castor3d::OverlayWPtr m_hudUpgrade;
		castor3d::TextOverlayWPtr m_lives;
		castor3d::TextOverlayWPtr m_gold;
		castor3d::TextOverlayWPtr m_wave;
		castor3d::TextOverlayWPtr m_kills;
		castor3d::TextOverlayWPtr m_enemyLife;
		castor3d::TextOverlayWPtr m_enemyBounty;
		castor3d::TextOverlayWPtr m_towerSpeed;
		castor3d::TextOverlayWPtr m_towerRange;
		castor3d::TextOverlayWPtr m_towerDamage;
		SoundSource const & m_buttonSound;
	};
}

#endif
