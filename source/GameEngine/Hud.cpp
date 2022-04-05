#include "Hud.hpp"

#include "Game.hpp"

#include <Castor3D/Engine.hpp>
#include <Castor3D/Cache/OverlayCache.hpp>
#include <Castor3D/Event/UserInput/UserInputListener.hpp>
#include <Castor3D/Overlay/Overlay.hpp>
#include <Castor3D/Overlay/TextOverlay.hpp>
#include <Castor3D/Scene/Scene.hpp>

using namespace castor;
using namespace castor3d;

namespace orastus
{
	namespace
	{
		TextOverlaySPtr getTextOverlay( OverlayCache & p_cache
			, String const & p_name )
		{
			TextOverlaySPtr l_return;
			OverlaySPtr l_overlay = p_cache.find( p_name ).lock();

			if ( l_overlay )
			{
				l_return = l_overlay->getTextOverlay();
			}

			return l_return;
		}

		template< typename T >
		void doUpdateText( Ecs const & p_ecs
			, Entity const & p_entity
			, ComponentId const & p_component
			, TextOverlayWPtr const & p_text )
		{
			auto & l_data = p_ecs.getComponentData< T >( p_entity
				, p_ecs.getComponent( p_component ) );
			auto l_text = p_text.lock();

			if ( l_text )
			{
				l_text->setVisible( true );
				l_text->setCaption( toString( l_data.getValue() ) );
			}
		}
	}

	Hud::Hud( Game & p_game
		, Scene const & p_scene )
		: m_game{ p_game }
		, m_scene{ p_scene }
		, m_lives{ getTextOverlay( p_scene.getEngine()->getOverlayCache(), cuT( "LivesValue" ) ) }
		, m_ore{ getTextOverlay( p_scene.getEngine()->getOverlayCache(), cuT( "OreValue" ) ) }
		, m_level{ getTextOverlay( p_scene.getEngine()->getOverlayCache(), cuT( "LevelValue" ) ) }
		, m_kills{ getTextOverlay( p_scene.getEngine()->getOverlayCache(), cuT( "KillsValue" ) ) }
		, m_enemyLife{ getTextOverlay( p_scene.getEngine()->getOverlayCache(), cuT( "MonsterLifeValue" ) ) }
		, m_enemyBounty{ getTextOverlay( p_scene.getEngine()->getOverlayCache(), cuT( "MonsterBountyValue" ) ) }
		, m_towerSpeed{ getTextOverlay( p_scene.getEngine()->getOverlayCache(), cuT( "TowerSpeedValue" ) ) }
		, m_towerRange{ getTextOverlay( p_scene.getEngine()->getOverlayCache(), cuT( "TowerRangeValue" ) ) }
		, m_towerDamage{ getTextOverlay( p_scene.getEngine()->getOverlayCache(), cuT( "TowerDamageValue" ) ) }
	{
	}

	void Hud::initialise()
	{
		m_scene.getEngine()->postEvent( makeCpuFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.getEngine()->getOverlayCache();
				l_cache.find( cuT( "TitlePanel" ) ).lock()->setVisible( true );
				l_cache.find( cuT( "HUDResources" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDScore" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDDetails" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDPause" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "GameEndPanel" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HelpPanel" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDBuild1" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDBuild2" ) ).lock()->setVisible( false );
			} ) );
	}

	void Hud::start()
	{
		m_scene.getEngine()->postEvent( makeCpuFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.getEngine()->getOverlayCache();
				l_cache.find( cuT( "TitlePanel" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDResources" ) ).lock()->setVisible( true );
				l_cache.find( cuT( "HUDScore" ) ).lock()->setVisible( true );
				l_cache.find( cuT( "HUDDetails" ) ).lock()->setVisible( true );
				l_cache.find( cuT( "HUDPause" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "GameEndPanel" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HelpPanel" ) ).lock()->setVisible( false );
				update();
			} ) );
	}

	void Hud::pause()
	{
		m_scene.getEngine()->postEvent( makeCpuFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.getEngine()->getOverlayCache();
				l_cache.find( cuT( "TitlePanel" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDResources" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDScore" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDDetails" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDPause" ) ).lock()->setVisible( true );
				l_cache.find( cuT( "GameEndPanel" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HelpPanel" ) ).lock()->setVisible( false );
			} ) );
	}

	void Hud::resume()
	{
		m_scene.getEngine()->postEvent( makeCpuFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.getEngine()->getOverlayCache();
				l_cache.find( cuT( "TitlePanel" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDResources" ) ).lock()->setVisible( true );
				l_cache.find( cuT( "HUDScore" ) ).lock()->setVisible( true );
				l_cache.find( cuT( "HUDDetails" ) ).lock()->setVisible( true );
				l_cache.find( cuT( "HUDPause" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "GameEndPanel" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HelpPanel" ) ).lock()->setVisible( false );
			} ) );
	}

	void Hud::help()
	{
		m_scene.getEngine()->postEvent( makeCpuFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.getEngine()->getOverlayCache();
				l_cache.find( cuT( "TitlePanel" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDResources" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDScore" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDDetails" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDPause" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "GameEndPanel" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HelpPanel" ) ).lock()->setVisible( true );
			} ) );
	}

	void Hud::gameOver()
	{
		m_scene.getEngine()->postEvent( makeCpuFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.getEngine()->getOverlayCache();
				l_cache.find( cuT( "TitlePanel" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDResources" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDScore" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDDetails" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDPause" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "GameEndPanel" ) ).lock()->setVisible( true );
				l_cache.find( cuT( "HelpPanel" ) ).lock()->setVisible( false );

				//getTextOverlay( m_scene.getEngine()->getOverlayCache(), cuT( "ResultLevelValue" ) )->setCaption( StringStream() << m_game.getWave() );
				//getTextOverlay( m_scene.getEngine()->getOverlayCache(), cuT( "ResultKillsValue" ) )->setCaption( StringStream() << m_game.getKills() );
			} ) );
	}

	void Hud::showBuild()
	{
		auto l_userInput = m_scene.getEngine()->getUserInputListener();
		l_userInput->fireMaterialEvent( cuT( "Build1ThumbFront" ), cuT( "HUDThumbShortRange" ) );
		l_userInput->fireMaterialEvent( cuT( "Build2ThumbFront" ), cuT( "HUDThumbLongRange" ) );
		l_userInput->fireTextEvent( cuT( "Build1Title" ), cuT( "Short range" ) );
		l_userInput->fireTextEvent( cuT( "Build2Title" ), cuT( "Long range" ) );
		l_userInput->registerClickAction( cuT( "Build1ThumbFrontButton" )
			, [this]()
			{
				m_game.createShortRangeTower();
			} );
		l_userInput->registerClickAction( cuT( "Build2ThumbFrontButton" )
			, [this]()
			{
				m_game.createLongRangeTower();
			} );
		m_scene.getEngine()->postEvent( makeCpuFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.getEngine()->getOverlayCache();
				l_cache.find( cuT( "HUDBuild1" ) ).lock()->setVisible( true );
				l_cache.find( cuT( "HUDBuild2" ) ).lock()->setVisible( true );
			} ) );
	}

	void Hud::hideBuild()
	{
		m_scene.getEngine()->postEvent( makeCpuFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.getEngine()->getOverlayCache();
				l_cache.find( cuT( "HUDBuild1" ) ).lock()->setVisible( false );
				l_cache.find( cuT( "HUDBuild2" ) ).lock()->setVisible( false );
			} ) );
		auto l_userInput = m_scene.getEngine()->getUserInputListener();
		l_userInput->unregisterClickAction( cuT( "Build1ThumbFrontButton" ) );
		l_userInput->unregisterClickAction( cuT( "Build2ThumbFrontButton" ) );
	}

	void Hud::updateTowerInfo( Ecs const & p_ecs
		, Entity const & p_entity )
	{
		if ( p_entity )
		{
			m_scene.getEngine()->postEvent( makeCpuFunctorEvent( EventType::ePreRender
				, [this, &p_ecs, p_entity]()
				{
					doUpdateText< uint32_t >( p_ecs, p_entity, Ecs::DamageComponent, m_towerDamage );
					doUpdateText< Milliseconds >( p_ecs, p_entity, Ecs::CooldownComponent, m_towerSpeed );
					doUpdateText< float >( p_ecs, p_entity, Ecs::RangeComponent, m_towerRange );
				} ) );
		}
		else
		{
			m_scene.getEngine()->postEvent( makeCpuFunctorEvent( EventType::ePreRender
				, [this]()
				{
					m_towerDamage.lock()->setVisible( false );
					m_towerSpeed.lock()->setVisible( false );
					m_towerRange.lock()->setVisible( false );
				} ) );
		}
	}

	void Hud::update()
	{
		//auto l_text = m_lives.lock();

		//if ( l_text )
		//{
		//	l_text->setCaption( StringStream() << m_game.getLives() );
		//}

		//l_text = m_ore.lock();

		//if ( l_text )
		//{
		//	l_text->setCaption( StringStream() << m_game.getOre() );
		//}

		//l_text = m_level.lock();

		//if ( l_text )
		//{
		//	l_text->setCaption( StringStream() << m_game.getWave() );
		//}

		//l_text = m_kills.lock();

		//if ( l_text )
		//{
		//	l_text->setCaption( StringStream() << m_game.getKills() );
		//}

		//l_text = m_enemyLife.lock();

		//if ( l_text )
		//{
		//	l_text->setCaption( StringStream() << m_game.getEnemiesLife() );
		//}

		//l_text = m_enemyBounty.lock();

		//if ( l_text )
		//{
		//	l_text->setCaption( StringStream() << m_game.getEnemiesBounty() );
		//}
	}
}
