#include "GameEngine/Hud.hpp"

#include "GameEngine/Game.hpp"

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
		castor3d::TextOverlaySPtr getTextOverlay( castor3d::OverlayCache & cache
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
		, castor3d::Scene const & scene )
		: m_game{ game }
		, m_scene{ scene }
		, m_lives{ getTextOverlay( m_scene.getEngine()->getOverlayCache(), cuT( "LivesValue" ) ) }
		, m_ore{ getTextOverlay( m_scene.getEngine()->getOverlayCache(), cuT( "OreValue" ) ) }
		, m_level{ getTextOverlay( m_scene.getEngine()->getOverlayCache(), cuT( "LevelValue" ) ) }
		, m_kills{ getTextOverlay( m_scene.getEngine()->getOverlayCache(), cuT( "KillsValue" ) ) }
		, m_enemyLife{ getTextOverlay( m_scene.getEngine()->getOverlayCache(), cuT( "MonsterLifeValue" ) ) }
		, m_enemyBounty{ getTextOverlay( m_scene.getEngine()->getOverlayCache(), cuT( "MonsterBountyValue" ) ) }
		, m_towerSpeed{ getTextOverlay( m_scene.getEngine()->getOverlayCache(), cuT( "TowerSpeedValue" ) ) }
		, m_towerRange{ getTextOverlay( m_scene.getEngine()->getOverlayCache(), cuT( "TowerRangeValue" ) ) }
		, m_towerDamage{ getTextOverlay( m_scene.getEngine()->getOverlayCache(), cuT( "TowerDamageValue" ) ) }
	{
	}

	void Hud::initialise()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				auto & cache = m_scene.getEngine()->getOverlayCache();
				cache.find( cuT( "TitlePanel" ) ).lock()->setVisible( true );
				cache.find( cuT( "HUDResources" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDScore" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDDetails" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDPause" ) ).lock()->setVisible( false );
				cache.find( cuT( "GameEndPanel" ) ).lock()->setVisible( false );
				cache.find( cuT( "HelpPanel" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDBuild" ) ).lock()->setVisible( false );
			} ) );
	}

	void Hud::start()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				auto & cache = m_scene.getEngine()->getOverlayCache();
				cache.find( cuT( "TitlePanel" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDResources" ) ).lock()->setVisible( true );
				cache.find( cuT( "HUDScore" ) ).lock()->setVisible( true );
				cache.find( cuT( "HUDDetails" ) ).lock()->setVisible( true );
				cache.find( cuT( "HUDPause" ) ).lock()->setVisible( false );
				cache.find( cuT( "GameEndPanel" ) ).lock()->setVisible( false );
				cache.find( cuT( "HelpPanel" ) ).lock()->setVisible( false );
				update();
			} ) );
	}

	void Hud::pause()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				auto & cache = m_scene.getEngine()->getOverlayCache();
				cache.find( cuT( "TitlePanel" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDResources" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDScore" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDDetails" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDPause" ) ).lock()->setVisible( true );
				cache.find( cuT( "GameEndPanel" ) ).lock()->setVisible( false );
				cache.find( cuT( "HelpPanel" ) ).lock()->setVisible( false );
			} ) );
	}

	void Hud::resume()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				auto & cache = m_scene.getEngine()->getOverlayCache();
				cache.find( cuT( "TitlePanel" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDResources" ) ).lock()->setVisible( true );
				cache.find( cuT( "HUDScore" ) ).lock()->setVisible( true );
				cache.find( cuT( "HUDDetails" ) ).lock()->setVisible( true );
				cache.find( cuT( "HUDPause" ) ).lock()->setVisible( false );
				cache.find( cuT( "GameEndPanel" ) ).lock()->setVisible( false );
				cache.find( cuT( "HelpPanel" ) ).lock()->setVisible( false );
			} ) );
	}

	void Hud::help()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				auto & cache = m_scene.getEngine()->getOverlayCache();
				cache.find( cuT( "TitlePanel" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDResources" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDScore" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDDetails" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDPause" ) ).lock()->setVisible( false );
				cache.find( cuT( "GameEndPanel" ) ).lock()->setVisible( false );
				cache.find( cuT( "HelpPanel" ) ).lock()->setVisible( true );
			} ) );
	}

	void Hud::gameOver()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				auto & cache = m_scene.getEngine()->getOverlayCache();
				cache.find( cuT( "TitlePanel" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDResources" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDScore" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDDetails" ) ).lock()->setVisible( false );
				cache.find( cuT( "HUDPause" ) ).lock()->setVisible( false );
				cache.find( cuT( "GameEndPanel" ) ).lock()->setVisible( true );
				cache.find( cuT( "HelpPanel" ) ).lock()->setVisible( false );

				getTextOverlay( m_scene.getEngine()->getOverlayCache(), cuT( "ResultLevelValue" ) )->setCaption( castor::string::toString( m_game.getWave() ) );
				getTextOverlay( m_scene.getEngine()->getOverlayCache(), cuT( "ResultKillsValue" ) )->setCaption( castor::string::toString( m_game.getKills() ) );
			} ) );
	}

	void Hud::showBuild()
	{
		auto userInput = m_scene.getEngine()->getUserInputListener();
		userInput->registerClickAction( cuT( "Build/ShortRange/Button" )
			, [this]()
			{
				m_game.createShortRangeTower();
			} );
		userInput->registerClickAction( cuT( "Build/Splash/Button" )
			, [this]()
			{
				m_game.createLongRangeTower();
			} );
		userInput->registerClickAction( cuT( "HUDBuild" )
			, []()
			{
				// This is to prevent click behind the build overlay.
			} );
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				auto & cache = m_scene.getEngine()->getOverlayCache();
				cache.find( cuT( "HUDBuild" ) ).lock()->setVisible( true );
			} ) );
	}

	void Hud::hideBuild()
	{
		m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
			, [this]()
			{
				auto & cache = m_scene.getEngine()->getOverlayCache();
				cache.find( cuT( "HUDBuild" ) ).lock()->setVisible( false );
			} ) );
		auto userInput = m_scene.getEngine()->getUserInputListener();
		userInput->unregisterClickAction( cuT( "Build/ShortRange/Button" ) );
		userInput->unregisterClickAction( cuT( "Build/Splash/Button" ) );
		userInput->unregisterClickAction( cuT( "HUDBuild" ) );
	}

	void Hud::updateTowerInfo( Ecs const & ecs
		, Entity const & entity )
	{
		if ( entity )
		{
			m_scene.getEngine()->postEvent( castor3d::makeCpuFunctorEvent( castor3d::EventType::ePreRender
				, [this, &ecs, entity]()
				{
					doUpdateText< uint32_t >( ecs, entity, Ecs::DamageComponent, m_towerDamage );
					doUpdateText< Milliseconds >( ecs, entity, Ecs::CooldownComponent, m_towerSpeed );
					doUpdateText< float >( ecs, entity, Ecs::RangeComponent, m_towerRange );
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

		//text = m_ore.lock();

		//if ( text )
		//{
		//	text->setCaption( StringStream() << m_game.getOre() );
		//}

		text = m_level.lock();

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
