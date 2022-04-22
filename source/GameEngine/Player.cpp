#include "GameEngine/Player.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/GridCell.hpp"
#include "GameEngine/ECS/Entity.hpp"

#include <Castor3D/Engine.hpp>

namespace orastus
{
	//*********************************************************************************************

	void Player::SelectedEntity::select( Entity const & pentity
		, castor3d::GeometrySPtr pgeometry )
	{
		unselect();

		if ( pentity && pgeometry )
		{
			entity = pentity;
			geometry = pgeometry;
			materials.clear();
			auto & cache = geometry->getScene()->getEngine()->getMaterialCache();

			for ( auto submesh : *geometry->getMesh().lock() )
			{
				auto oldMat = geometry->getMaterial( *submesh );
				auto newMat = cache.find( oldMat->getName() + cuT( "_sel" ) ).lock();
				CU_Require( newMat );
				geometry->setMaterial( *submesh, newMat.get() );
				materials.push_back( oldMat );
			}
		}
	}

	void Player::SelectedEntity::unselect()
	{
		if ( entity && geometry )
		{
			uint32_t index{ 0u };
			auto mesh = geometry->getMesh().lock();

			for ( auto material : materials )
			{
				geometry->setMaterial( *mesh->getSubmesh( index++ ), material );
			}

			materials.clear();
			geometry.reset();
			entity = Entity{};
		}
	}

	//*********************************************************************************************

	Player::Player( Game & game
		, Ecs & ecs
		, Hud & hud
		, uint32_t initialGold )
		: m_game{ game }
		, m_ecs{ ecs }
		, m_hud{ hud }
		, m_gold{ initialGold }
	{
	}

	bool Player::selectEntity( Entity const & entity )
	{
		bool result = false;

		if ( m_ecs.hasComponent( entity, m_ecs.getComponent( Ecs::TowerStateComponent ) ) )
		{
			result = true;
			doSelectTower( entity );
		}
		else
		{
			doSelectMapBlock( entity );
		}

		return result;
	}

	Entity Player::select( castor3d::GeometrySPtr geometry )
	{
		Entity result;
		auto entity = m_ecs.findEntity( m_ecs.getComponent( Ecs::GeometryComponent ), geometry );
		m_selectedCell = &doFindCell( entity );
		entity = m_selectedCell->entity;

		if ( m_ecs.hasComponent( entity, m_ecs.getComponent( Ecs::PickableComponent ) )
			&& m_ecs.getComponentData< bool >( entity, m_ecs.getComponent( Ecs::PickableComponent ) ).getValue() )
		{
			if ( selectEntity( entity ) )
			{
				result = entity;
			}
		}
		else
		{
			unselect();
		}

		return result;
	}

	void Player::unselect()
	{
		m_hud.hideBuild();
		m_hud.updateTowerInfo( m_ecs, Entity{} );
		m_selectedCell = nullptr;

		m_game.getScene().getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				m_selectedBlock.unselect();
				m_selectedEntity.unselect();
			} ) );
	}

	void Player::earnGold( uint32_t value )
	{
		m_gold += value;
	}

	bool Player::canSpendGold( uint32_t value )const
	{
		return m_gold >= value;
	}

	void Player::spendGold( uint32_t value )
	{
		m_gold -= value;
	}

	bool Player::isEmptyCellSelected()const
	{
		return m_selectedCell
			&& *m_selectedCell
			&& m_selectedCell->state == GridCell::State::eEmpty;
	}

	void Player::doSelectMapBlock( Entity const & entity )
	{
		auto geometry = doGetGeometry( entity );

		if ( geometry )
		{
			m_hud.updateTowerInfo( m_ecs, Entity{} );
			m_hud.showBuild();
			m_game.getScene().getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePostRender
				, [this, geometry, entity]()
				{
					m_selectedBlock.select( entity, geometry );
					m_selectedEntity.unselect();
				} ) );
		}
	}

	 void Player::doSelectTower( Entity const & entity )
	{
		auto geometry = doGetGeometry( entity );

		if ( geometry )
		{
			m_hud.updateTowerInfo( m_ecs, entity );
			m_hud.hideBuild();
			m_game.getScene().getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePostRender
				, [this, geometry, entity]()
				{
					m_selectedBlock.unselect();
					m_selectedEntity.select( entity, geometry );
				} ) );
		}
	}

	 castor3d::GeometrySPtr Player::doGetGeometry( Entity const & entity )
	{
		 castor3d::GeometrySPtr result;

		try
		{
			result = m_ecs.getComponentData< castor3d::GeometrySPtr >( entity
				, m_ecs.getComponent( Ecs::GeometryComponent ) ).getValue();
		}
		catch ( Ecs::ComponentDataMatchException & exc )
		{
			castor::Logger::logWarning( exc.what() );
		}

		return result;
	}

	GridCell & Player::doFindCell( Entity const & entity )
	{
		static GridCell invalidCell{ 0u, 0u, GridCell::State::eInvalid };
		auto & cellComponent = m_ecs.getComponent( Ecs::CellComponent );

		if ( m_ecs.hasComponent( entity, cellComponent ) )
		{
			return *m_ecs.getComponentData< GridCell * >( entity, cellComponent ).getValue();
		}

		return invalidCell;
	}

	//*********************************************************************************************
}
