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
		castor3d::TextOverlayRPtr getTextOverlay( castor3d::OverlayCache const & cache
			, castor::String const & name )
		{
			castor3d::TextOverlayRPtr result{};
			
			if ( auto overlay = cache.tryFind( name ) )
			{
				result = overlay->getTextOverlay();
			}

			return result;
		}

		template< typename T >
		void doUpdateText( Ecs const & ecs
			, Entity const & entity
			, ComponentId const & component
			, castor3d::TextOverlayRPtr const & ptext )
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
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::CpuEventType::ePreGpuStep
			, [this]()
			{
				m_titlePanel->setVisible( true );
				m_hudResources->setVisible( false );
				m_hudScore->setVisible( false );
				m_hudDetails->setVisible( false );
				m_hudPause->setVisible( false );
				m_gameEndPanel->setVisible( false );
				m_helpPanel->setVisible( false );
				m_hudBuild->setVisible( false );
				m_hudUpgrade->setVisible( false );
			} ) );
	}

	void Hud::start()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::CpuEventType::ePreGpuStep
			, [this]()
			{
				m_titlePanel->setVisible( false );
				m_hudResources->setVisible( true );
				m_hudScore->setVisible( true );
				m_hudDetails->setVisible( true );
				m_hudPause->setVisible( false );
				m_gameEndPanel->setVisible( false );
				m_helpPanel->setVisible( false );
				update();
			} ) );
	}

	void Hud::pause()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::CpuEventType::ePreGpuStep
			, [this]()
			{
				m_titlePanel->setVisible( false );
				m_hudResources->setVisible( false );
				m_hudScore->setVisible( false );
				m_hudDetails->setVisible( false );
				m_hudPause->setVisible( true );
				m_gameEndPanel->setVisible( false );
				m_helpPanel->setVisible( false );
			} ) );
	}

	void Hud::resume()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::CpuEventType::ePreGpuStep
			, [this]()
			{
				m_titlePanel->setVisible( false );
				m_hudResources->setVisible( true );
				m_hudScore->setVisible( true );
				m_hudDetails->setVisible( true );
				m_hudPause->setVisible( false );
				m_gameEndPanel->setVisible( false );
				m_helpPanel->setVisible( false );
			} ) );
	}

	void Hud::help()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::CpuEventType::ePreGpuStep
			, [this]()
			{
				m_titlePanel->setVisible( false );
				m_hudResources->setVisible( false );
				m_hudScore->setVisible( false );
				m_hudDetails->setVisible( false );
				m_hudPause->setVisible( false );
				m_gameEndPanel->setVisible( false );
				m_helpPanel->setVisible( true );
			} ) );
	}

	void Hud::gameOver()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::CpuEventType::ePreGpuStep
			, [this]()
			{
				m_titlePanel->setVisible( false );
				m_hudResources->setVisible( false );
				m_hudScore->setVisible( false );
				m_hudDetails->setVisible( false );
				m_hudPause->setVisible( false );
				m_gameEndPanel->setVisible( true );
				m_helpPanel->setVisible( false );

				auto & cache = m_scene.getOverlayCache();
				getTextOverlay( cache, cuT( "ResultWaveValue" ) )->setCaption( castor::toUtf8U32String( castor::string::toString( m_game.getWave() ) ) );
				getTextOverlay( cache, cuT( "ResultKillsValue" ) )->setCaption( castor::toUtf8U32String( castor::string::toString( m_game.getKills() ) ) );
			} ) );
	}

	void Hud::showBuild()
	{
		auto userInput = m_scene.getEngine()->getUserInputListener();

		if ( m_game.getPlayer().canSpendGold( DirectTower::GoldCost ) )
		{
			userInput->enableHandler( cuT( "HUDBuild/Direct/Button" ) );
			userInput->registerClickAction( cuT( "HUDBuild/Direct/Button" )
				, [this]()
				{
					m_buttonSound.play();
					m_game.createDirectTower();
				} );
		}
		else
		{
			userInput->disableHandler( cuT( "HUDBuild/Direct/Button" ) );
		}

		if ( m_game.getPlayer().canSpendGold( SplashTower::GoldCost ) )
		{
			userInput->enableHandler( cuT( "HUDBuild/Splash/Button" ) );
			userInput->registerClickAction( cuT( "HUDBuild/Splash/Button" )
				, [this]()
				{
					m_buttonSound.play();
					m_game.createSplashTower();
				} );
		}
		else
		{
			userInput->disableHandler( cuT( "HUDBuild/Splash/Button" ) );
		}

		userInput->registerClickAction( cuT( "HUDBuild" )
			, []()
			{
				// This is to prevent click behind the build overlay.
			} );
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::CpuEventType::ePreGpuStep
			, [this]()
			{
				m_hudBuild->setVisible( true );
			} ) );
	}

	void Hud::hideBuild()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::CpuEventType::ePreGpuStep
			, [this]()
			{
				m_hudBuild->setVisible( false );
			} ) );
		auto userInput = m_scene.getEngine()->getUserInputListener();
		userInput->unregisterClickAction( cuT( "HUDBuild/Direct/Button" ) );
		userInput->unregisterClickAction( cuT( "HUDBuild/Splash/Button" ) );
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
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::CpuEventType::ePreGpuStep
			, [this]()
			{
				m_hudUpgrade->setVisible( true );
			} ) );
	}

	void Hud::hideUpgrade()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::CpuEventType::ePreGpuStep
			, [this]()
			{
				m_hudUpgrade->setVisible( false );
			} ) );
		auto userInput = m_scene.getEngine()->getUserInputListener();
		userInput->unregisterClickAction( cuT( "HUDUpgrade" ) );
	}

	void Hud::updateTowerInfo( Ecs const & ecs
		, Entity const & entity )
	{
		if ( entity )
		{
			m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::CpuEventType::ePreGpuStep
				, [this, &ecs, entity]()
				{
					auto & towerData = ecs.getComponentData< TowerData >( entity, ecs.getComponent( Ecs::TowerStateComponent ) ).getValue();
					m_towerDamage->setCaption( castor::toUtf8U32String( castor::string::toString( towerData.category->getDamage() ) ) );
					m_towerSpeed->setCaption( castor::toUtf8U32String( castor::string::toString( towerData.category->getCooldown() ) ) );
					m_towerRange->setCaption( castor::toUtf8U32String( castor::string::toString( towerData.category->getRange() ) ) );
					m_towerDamage->setVisible( true );
					m_towerSpeed->setVisible( true );
					m_towerRange->setVisible( true );
				} ) );
		}
		else
		{
			m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::CpuEventType::ePreGpuStep
				, [this]()
				{
					m_towerDamage->setVisible( false );
					m_towerSpeed->setVisible( false );
					m_towerRange->setVisible( false );
				} ) );
		}
	}

	void Hud::update()
	{
		if ( auto text = m_lives )
		{
			text->setCaption( castor::toUtf8U32String( castor::string::toString( m_game.getLives() ) ) );
		}

		if ( auto text = m_gold )
		{
			text->setCaption( castor::toUtf8U32String( castor::string::toString( m_game.getPlayer().getGold() ) ) );
		}

		if ( auto text = m_wave )
		{
			text->setCaption( castor::toUtf8U32String( castor::string::toString( m_game.getWave() ) ) );
		}

		if ( auto text = m_kills )
		{
			text->setCaption( castor::toUtf8U32String( castor::string::toString( m_game.getKills() ) ) );
		}

		if ( auto text = m_enemyLife )
		{
			text->setCaption( castor::toUtf8U32String( castor::string::toString( m_game.getEnemiesLife() ) ) );
		}

		if ( auto text = m_enemyBounty )
		{
			text->setCaption( castor::toUtf8U32String( castor::string::toString( m_game.getEnemiesBounty() ) ) );
		}
	}
}
