#include "GameEngine/Hud.hpp"

#include "GameEngine/Game.hpp"
#include "GameEngine/Sound.hpp"
#include "GameEngine/Ecs/Tower.hpp"

#include <Castor3D/Engine.hpp>
#include <Castor3D/Cache/OverlayCache.hpp>
#include <Castor3D/Event/UserInput/UserInputListener.hpp>
#include <Castor3D/Overlay/Overlay.hpp>
#include <Castor3D/Overlay/TextOverlay.hpp>
#include <Castor3D/Scene/Scene.hpp>

namespace orastus
{
	namespace
	{
		castor3d::TextOverlaySPtr getTextOverlay( castor3d::OverlayCache const & cache
			, castor::String const & name )
		{
			castor3d::TextOverlaySPtr result;
			auto overlay = cache.find( name ).lock();

			if ( overlay )
			{
				result = overlay->getTextOverlay();
			}

			return result;
		}

		template< typename T >
		void doUpdateText( Ecs const & ecs
			, Entity const & entity
			, ComponentId const & component
			, castor3d::TextOverlayWPtr const & ptext )
		{
			auto & data = ecs.getComponentData< T >( entity
				, ecs.getComponent( component ) );
			auto text = ptext.lock();

			if ( text )
			{
				text->setVisible( true );
				text->setCaption( toString( data.getValue() ) );
			}
		}
	}

	Hud::Hud( Game & game
		, castor::Path const & dataFolder
		, castor3d::Scene const & scene )
		: m_game{ game }
		, m_scene{ scene }
		, m_titlePanel{ m_scene.getOverlayCache().find( cuT( "TitlePanel" ) ) }
		, m_gameEndPanel{ m_scene.getOverlayCache().find( cuT( "GameEndPanel" ) ) }
		, m_helpPanel{ m_scene.getOverlayCache().find( cuT( "HelpPanel" ) ) }
		, m_hudResources{ m_scene.getOverlayCache().find( cuT( "HUDResources" ) ) }
		, m_hudScore{ m_scene.getOverlayCache().find( cuT( "HUDScore" ) ) }
		, m_hudDetails{ m_scene.getOverlayCache().find( cuT( "HUDDetails" ) ) }
		, m_hudPause{ m_scene.getOverlayCache().find( cuT( "HUDPause" ) ) }
		, m_hudBuild{ m_scene.getOverlayCache().find( cuT( "HUDBuild" ) ) }
		, m_hudUpgrade{ m_scene.getOverlayCache().find( cuT( "HUDUpgrade" ) ) }
		, m_lives{ getTextOverlay( m_scene.getOverlayCache(), cuT( "LivesValue" ) ) }
		, m_gold{ getTextOverlay( m_scene.getOverlayCache(), cuT( "GoldValue" ) ) }
		, m_wave{ getTextOverlay( m_scene.getOverlayCache(), cuT( "WaveValue" ) ) }
		, m_kills{ getTextOverlay( m_scene.getOverlayCache(), cuT( "KillsValue" ) ) }
		, m_enemyLife{ getTextOverlay( m_scene.getOverlayCache(), cuT( "MonsterLifeValue" ) ) }
		, m_enemyBounty{ getTextOverlay( m_scene.getOverlayCache(), cuT( "MonsterBountyValue" ) ) }
		, m_towerSpeed{ getTextOverlay( m_scene.getOverlayCache(), cuT( "TowerSpeedValue" ) ) }
		, m_towerRange{ getTextOverlay( m_scene.getOverlayCache(), cuT( "TowerRangeValue" ) ) }
		, m_towerDamage{ getTextOverlay( m_scene.getOverlayCache(), cuT( "TowerDamageValue" ) ) }
		, m_buttonSound{ game.addSound( dataFolder / "Sounds" / "hud_button.wav", 1u ).createSource( false ) }
	{
	}

	void Hud::initialise()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				m_titlePanel.lock()->setVisible( true );
				m_hudResources.lock()->setVisible( false );
				m_hudScore.lock()->setVisible( false );
				m_hudDetails.lock()->setVisible( false );
				m_hudPause.lock()->setVisible( false );
				m_gameEndPanel.lock()->setVisible( false );
				m_helpPanel.lock()->setVisible( false );
				m_hudBuild.lock()->setVisible( false );
				m_hudUpgrade.lock()->setVisible( false );
			} ) );
	}

	void Hud::start()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				m_titlePanel.lock()->setVisible( false );
				m_hudResources.lock()->setVisible( true );
				m_hudScore.lock()->setVisible( true );
				m_hudDetails.lock()->setVisible( true );
				m_hudPause.lock()->setVisible( false );
				m_gameEndPanel.lock()->setVisible( false );
				m_helpPanel.lock()->setVisible( false );
				update();
			} ) );
	}

	void Hud::pause()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				m_titlePanel.lock()->setVisible( false );
				m_hudResources.lock()->setVisible( false );
				m_hudScore.lock()->setVisible( false );
				m_hudDetails.lock()->setVisible( false );
				m_hudPause.lock()->setVisible( true );
				m_gameEndPanel.lock()->setVisible( false );
				m_helpPanel.lock()->setVisible( false );
			} ) );
	}

	void Hud::resume()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				m_titlePanel.lock()->setVisible( false );
				m_hudResources.lock()->setVisible( true );
				m_hudScore.lock()->setVisible( true );
				m_hudDetails.lock()->setVisible( true );
				m_hudPause.lock()->setVisible( false );
				m_gameEndPanel.lock()->setVisible( false );
				m_helpPanel.lock()->setVisible( false );
			} ) );
	}

	void Hud::help()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				m_titlePanel.lock()->setVisible( false );
				m_hudResources.lock()->setVisible( false );
				m_hudScore.lock()->setVisible( false );
				m_hudDetails.lock()->setVisible( false );
				m_hudPause.lock()->setVisible( false );
				m_gameEndPanel.lock()->setVisible( false );
				m_helpPanel.lock()->setVisible( true );
			} ) );
	}

	void Hud::gameOver()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				m_titlePanel.lock()->setVisible( false );
				m_hudResources.lock()->setVisible( false );
				m_hudScore.lock()->setVisible( false );
				m_hudDetails.lock()->setVisible( false );
				m_hudPause.lock()->setVisible( false );
				m_gameEndPanel.lock()->setVisible( true );
				m_helpPanel.lock()->setVisible( false );

				auto & cache = m_scene.getOverlayCache();
				getTextOverlay( cache, cuT( "ResultWaveValue" ) )->setCaption( castor::string::toString( m_game.getWave() ) );
				getTextOverlay( cache, cuT( "ResultKillsValue" ) )->setCaption( castor::string::toString( m_game.getKills() ) );
			} ) );
	}

	void Hud::showBuild()
	{
		auto userInput = m_scene.getEngine()->getUserInputListener();

		if ( m_game.getPlayer().canSpendGold( DirectTower::GoldCost ) )
		{
			userInput->enableHandler( cuT( "Build/Direct/Button" ) );
			userInput->registerClickAction( cuT( "Build/Direct/Button" )
				, [this]()
				{
					m_buttonSound.play();
					m_game.createDirectTower();
				} );
		}
		else
		{
			userInput->disableHandler( cuT( "Build/Direct/Button" ) );
		}

		if ( m_game.getPlayer().canSpendGold( SplashTower::GoldCost ) )
		{
			userInput->enableHandler( cuT( "Build/Splash/Button" ) );
			userInput->registerClickAction( cuT( "Build/Splash/Button" )
				, [this]()
				{
					m_buttonSound.play();
					m_game.createSplashTower();
				} );
		}
		else
		{
			userInput->disableHandler( cuT( "Build/Splash/Button" ) );
		}

		userInput->registerClickAction( cuT( "HUDBuild" )
			, []()
			{
				// This is to prevent click behind the build overlay.
			} );
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				m_hudBuild.lock()->setVisible( true );
			} ) );
	}

	void Hud::hideBuild()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				m_hudBuild.lock()->setVisible( false );
			} ) );
		auto userInput = m_scene.getEngine()->getUserInputListener();
		userInput->unregisterClickAction( cuT( "Build/Direct/Button" ) );
		userInput->unregisterClickAction( cuT( "Build/Splash/Button" ) );
		userInput->unregisterClickAction( cuT( "HUDBuild" ) );
	}

	void Hud::showUpgrade()
	{
		auto userInput = m_scene.getEngine()->getUserInputListener();
		userInput->registerClickAction( cuT( "HUDUpgrade" )
			, []()
			{
				// This is to prevent click behind the update overlay.
			} );
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				m_hudUpgrade.lock()->setVisible( true );
			} ) );
	}

	void Hud::hideUpgrade()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				m_hudUpgrade.lock()->setVisible( false );
			} ) );
		auto userInput = m_scene.getEngine()->getUserInputListener();
		userInput->unregisterClickAction( cuT( "HUDUpgrade" ) );
	}

	void Hud::updateTowerInfo( Ecs const & ecs
		, Entity const & entity )
	{
		if ( entity )
		{
			m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
				, [this, &ecs, entity]()
				{
					auto & towerData = ecs.getComponentData< TowerData >( entity, ecs.getComponent( Ecs::TowerStateComponent ) ).getValue();
					m_towerDamage.lock()->setCaption( castor::string::toString( towerData.category->getDamage() ) );
					m_towerSpeed.lock()->setCaption( castor::string::toString( towerData.category->getCooldown() ) );
					m_towerRange.lock()->setCaption( castor::string::toString( towerData.category->getRange() ) );
					m_towerDamage.lock()->setVisible( true );
					m_towerSpeed.lock()->setVisible( true );
					m_towerRange.lock()->setVisible( true );
				} ) );
		}
		else
		{
			m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
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
		auto text = m_lives.lock();

		if ( text )
		{
			text->setCaption( castor::string::toString( m_game.getLives() ) );
		}

		text = m_gold.lock();

		if ( text )
		{
			text->setCaption( castor::string::toString( m_game.getPlayer().getGold() ) );
		}

		text = m_wave.lock();

		if ( text )
		{
			text->setCaption( castor::string::toString( m_game.getWave() ) );
		}

		text = m_kills.lock();

		if ( text )
		{
			text->setCaption( castor::string::toString( m_game.getKills() ) );
		}

		text = m_enemyLife.lock();

		if ( text )
		{
			text->setCaption( castor::string::toString( m_game.getEnemiesLife() ) );
		}

		text = m_enemyBounty.lock();

		if ( text )
		{
			text->setCaption( castor::string::toString( m_game.getEnemiesBounty() ) );
		}
	}
}
