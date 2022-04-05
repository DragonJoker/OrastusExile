/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Hud_HPP___
#define ___EFO_Hud_HPP___

#include "GameEnginePrerequisites.hpp"

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
		*\param[in] p_game
		*	The game instance.
		*/
		EFO_API Hud( Game & p_game
			, castor3d::Scene const & p_scene );
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
		*	Updates the HUD.
		*/
		EFO_API void update();
		/**
		*\brief
		*	Updates the HUD tower info.
		*/
		EFO_API void updateTowerInfo( Ecs const & p_ecs
			, Entity const & p_entity );

	private:
		Game & m_game;
		castor3d::Scene const & m_scene;
		castor3d::TextOverlayWPtr m_lives;
		castor3d::TextOverlayWPtr m_ore;
		castor3d::TextOverlayWPtr m_level;
		castor3d::TextOverlayWPtr m_kills;
		castor3d::TextOverlayWPtr m_enemyLife;
		castor3d::TextOverlayWPtr m_enemyBounty;
		castor3d::TextOverlayWPtr m_towerSpeed;
		castor3d::TextOverlayWPtr m_towerRange;
		castor3d::TextOverlayWPtr m_towerDamage;
	};
}

#endif
