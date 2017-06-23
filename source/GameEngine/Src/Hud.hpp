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
			, Castor3D::Scene const & p_scene );
		/**
		*\brief
		*	Sets the HUD to initial state.
		*/
		EFO_API void Initialise();
		/**
		*\brief
		*	Shows "game running" HUD.
		*/
		EFO_API void Start();
		/**
		*\brief
		*	Shows "game paused" HUD.
		*/
		EFO_API void Pause();
		/**
		*\brief
		*	Shows "game running" HUD.
		*/
		EFO_API void Resume();
		/**
		*\brief
		*	Shows "help" HUD.
		*/
		EFO_API void Help();
		/**
		*\brief
		*	Shows "game over" HUD.
		*/
		EFO_API void GameOver();
		/**
		*\brief
		*	Shows the build tower options.
		*/
		EFO_API void ShowBuild();
		/**
		*\brief
		*	Shows the build tower options.
		*/
		EFO_API void HideBuild();
		/**
		*\brief
		*	Updates the HUD.
		*/
		EFO_API void Update();
		/**
		*\brief
		*	Updates the HUD tower info.
		*/
		EFO_API void UpdateTowerInfo( Ecs const & p_ecs
			, Entity const & p_entity );

	private:
		Game & m_game;
		Castor3D::Scene const & m_scene;
		Castor3D::TextOverlayWPtr m_lives;
		Castor3D::TextOverlayWPtr m_ore;
		Castor3D::TextOverlayWPtr m_level;
		Castor3D::TextOverlayWPtr m_kills;
		Castor3D::TextOverlayWPtr m_enemyLife;
		Castor3D::TextOverlayWPtr m_enemyBounty;
		Castor3D::TextOverlayWPtr m_towerSpeed;
		Castor3D::TextOverlayWPtr m_towerRange;
		Castor3D::TextOverlayWPtr m_towerDamage;
	};
}

#endif
