/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Player_HPP___
#define ___EFO_Player_HPP___

#include "GameEngine/GameEnginePrerequisites.hpp"
#include "GameEngine/ECS/Entity.hpp"

namespace orastus
{
	/**
	*\brief
	*	Player data owner.
	*/
	class Player
	{
	private:
		struct SelectedEntity
		{
			void select( Entity const & entity
				, castor3d::GeometryRPtr geometry );
			void unselect();

			Entity entity;
			castor3d::GeometryRPtr geometry;
			std::vector< castor3d::MaterialRPtr > materials;
			std::vector< castor3d::MaterialRPtr > selMaterials;
		};

	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] game
		*	The game.
		*\param[in] initialGold
		*	The initial gold wealth.
		*/
		EFO_API explicit Player( Game & game
			, Ecs & ecs
			, Hud & hud
			, uint32_t initialGold );
		/**
		*\brief
		*	Selects the entity for given geometry.
		*\param[in] geometry
		*	The geometry.
		*\return
		*	The tower entity at selected cell.
		*/
		EFO_API Entity select( castor3d::GeometryRPtr geometry );
		/**
		*\brief
		*	Selects the cell for given geometry.
		*\param[in] geometry
		*	The geometry.
		*\return
		*	The tower entity at selected cell.
		*/
		EFO_API bool selectEntity( Entity const & entity );
		/**@}*/
		/**
		*\brief
		*	Unselects the current selection.
		*/
		EFO_API void unselect();
		/**
		*\brief
		*	Earn gold.
		*/
		EFO_API void earnGold( uint32_t value );
		/**
		*\brief
		*	Tells if the player can spend the given amount of gold.
		*/
		EFO_API bool canSpendGold( uint32_t value )const;
		/**
		*\brief
		*	Spend the given amount of gold.
		*/
		EFO_API void spendGold( uint32_t value );
		/**
		*\brief
		*	Spend the given amount of gold.
		*/
		EFO_API bool isEmptyCellSelected()const;
		/**
		*\return
		*	The gold amount.
		*/
		uint32_t getGold()const
		{
			return m_gold;
		}
		/**
		*\return
		*	The currently selected cell.
		*/
		GridCell & getSelectedCell()const
		{
			CU_Require( m_selectedCell );
			return *m_selectedCell;
		}

	private:
		void doSelectMapBlock( Entity const & entity );
		void doSelectTower( Entity const & entity );
		castor3d::GeometryRPtr doGetGeometry( Entity const & entity );
		GridCell & doFindCell( Entity const & entity );

	private:
		Game & m_game;
		Ecs & m_ecs;
		Hud & m_hud;
		uint32_t m_gold{};
		GridCell * m_selectedCell{ nullptr };
		SelectedEntity m_selectedEntity;
		SelectedEntity m_selectedBlock;
	};
}

#endif
