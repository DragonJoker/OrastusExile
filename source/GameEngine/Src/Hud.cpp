#include "Hud.hpp"

#include "Game.hpp"

#include <Cache/OverlayCache.hpp>
#include <Event/UserInput/UserInputListener.hpp>
#include <Overlay/Overlay.hpp>
#include <Overlay/TextOverlay.hpp>
#include <Scene/Scene.hpp>

using namespace Castor;
using namespace Castor3D;

namespace orastus
{
	namespace
	{
		TextOverlaySPtr GetTextOverlay( OverlayCache & p_cache
			, String const & p_name )
		{
			TextOverlaySPtr l_return;
			OverlaySPtr l_overlay = p_cache.Find( p_name );

			if ( l_overlay )
			{
				l_return = l_overlay->GetTextOverlay();
			}

			return l_return;
		}

		template< typename T >
		void DoUpdateText( Ecs const & p_ecs
			, Entity const & p_entity
			, ComponentId const & p_component
			, TextOverlayWPtr const & p_text )
		{
			auto & l_data = p_ecs.GetComponentData< T >( p_entity
				, p_ecs.GetComponent( p_component ) );
			auto l_text = p_text.lock();

			if ( l_text )
			{
				l_text->SetVisible( true );
				l_text->SetCaption( ToString( l_data.GetValue() ) );
			}
		}
	}

	Hud::Hud( Game & p_game
		, Scene const & p_scene )
		: m_game{ p_game }
		, m_scene{ p_scene }
		, m_lives{ GetTextOverlay( p_scene.GetEngine()->GetOverlayCache(), cuT( "LivesValue" ) ) }
		, m_ore{ GetTextOverlay( p_scene.GetEngine()->GetOverlayCache(), cuT( "OreValue" ) ) }
		, m_level{ GetTextOverlay( p_scene.GetEngine()->GetOverlayCache(), cuT( "LevelValue" ) ) }
		, m_kills{ GetTextOverlay( p_scene.GetEngine()->GetOverlayCache(), cuT( "KillsValue" ) ) }
		, m_enemyLife{ GetTextOverlay( p_scene.GetEngine()->GetOverlayCache(), cuT( "MonsterLifeValue" ) ) }
		, m_enemyBounty{ GetTextOverlay( p_scene.GetEngine()->GetOverlayCache(), cuT( "MonsterBountyValue" ) ) }
		, m_towerSpeed{ GetTextOverlay( p_scene.GetEngine()->GetOverlayCache(), cuT( "TowerSpeedValue" ) ) }
		, m_towerRange{ GetTextOverlay( p_scene.GetEngine()->GetOverlayCache(), cuT( "TowerRangeValue" ) ) }
		, m_towerDamage{ GetTextOverlay( p_scene.GetEngine()->GetOverlayCache(), cuT( "TowerDamageValue" ) ) }
	{
	}

	void Hud::Initialise()
	{
		m_scene.GetEngine()->PostEvent( MakeFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.GetEngine()->GetOverlayCache();
				l_cache.Find( cuT( "TitlePanel" ) )->SetVisible( true );
				l_cache.Find( cuT( "HUDResources" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDScore" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDDetails" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDPause" ) )->SetVisible( false );
				l_cache.Find( cuT( "GameEndPanel" ) )->SetVisible( false );
				l_cache.Find( cuT( "HelpPanel" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDBuild1" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDBuild2" ) )->SetVisible( false );
			} ) );
	}

	void Hud::Start()
	{
		m_scene.GetEngine()->PostEvent( MakeFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.GetEngine()->GetOverlayCache();
				l_cache.Find( cuT( "TitlePanel" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDResources" ) )->SetVisible( true );
				l_cache.Find( cuT( "HUDScore" ) )->SetVisible( true );
				l_cache.Find( cuT( "HUDDetails" ) )->SetVisible( true );
				l_cache.Find( cuT( "HUDPause" ) )->SetVisible( false );
				l_cache.Find( cuT( "GameEndPanel" ) )->SetVisible( false );
				l_cache.Find( cuT( "HelpPanel" ) )->SetVisible( false );
				Update();
			} ) );
	}

	void Hud::Pause()
	{
		m_scene.GetEngine()->PostEvent( MakeFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.GetEngine()->GetOverlayCache();
				l_cache.Find( cuT( "TitlePanel" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDResources" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDScore" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDDetails" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDPause" ) )->SetVisible( true );
				l_cache.Find( cuT( "GameEndPanel" ) )->SetVisible( false );
				l_cache.Find( cuT( "HelpPanel" ) )->SetVisible( false );
			} ) );
	}

	void Hud::Resume()
	{
		m_scene.GetEngine()->PostEvent( MakeFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.GetEngine()->GetOverlayCache();
				l_cache.Find( cuT( "TitlePanel" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDResources" ) )->SetVisible( true );
				l_cache.Find( cuT( "HUDScore" ) )->SetVisible( true );
				l_cache.Find( cuT( "HUDDetails" ) )->SetVisible( true );
				l_cache.Find( cuT( "HUDPause" ) )->SetVisible( false );
				l_cache.Find( cuT( "GameEndPanel" ) )->SetVisible( false );
				l_cache.Find( cuT( "HelpPanel" ) )->SetVisible( false );
			} ) );
	}

	void Hud::Help()
	{
		m_scene.GetEngine()->PostEvent( MakeFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.GetEngine()->GetOverlayCache();
				l_cache.Find( cuT( "TitlePanel" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDResources" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDScore" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDDetails" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDPause" ) )->SetVisible( false );
				l_cache.Find( cuT( "GameEndPanel" ) )->SetVisible( false );
				l_cache.Find( cuT( "HelpPanel" ) )->SetVisible( true );
			} ) );
	}

	void Hud::GameOver()
	{
		m_scene.GetEngine()->PostEvent( MakeFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.GetEngine()->GetOverlayCache();
				l_cache.Find( cuT( "TitlePanel" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDResources" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDScore" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDDetails" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDPause" ) )->SetVisible( false );
				l_cache.Find( cuT( "GameEndPanel" ) )->SetVisible( true );
				l_cache.Find( cuT( "HelpPanel" ) )->SetVisible( false );

				//GetTextOverlay( m_scene.GetEngine()->GetOverlayCache(), cuT( "ResultLevelValue" ) )->SetCaption( StringStream() << m_game.GetWave() );
				//GetTextOverlay( m_scene.GetEngine()->GetOverlayCache(), cuT( "ResultKillsValue" ) )->SetCaption( StringStream() << m_game.GetKills() );
			} ) );
	}

	void Hud::ShowBuild()
	{
		auto & l_userInput = m_scene.GetEngine()->GetUserInputListener();
		auto & l_cache = m_scene.GetEngine()->GetOverlayCache();
		l_userInput->FireMaterialEvent( cuT( "Build1ThumbFront" ), cuT( "HUDThumbShortRange" ) );
		l_userInput->FireMaterialEvent( cuT( "Build2ThumbFront" ), cuT( "HUDThumbLongRange" ) );
		l_userInput->FireTextEvent( cuT( "Build1Title" ), cuT( "Short range" ) );
		l_userInput->FireTextEvent( cuT( "Build2Title" ), cuT( "Long range" ) );
		l_userInput->RegisterClickAction( cuT( "Build1ThumbFrontButton" )
			, [this]()
			{
				m_game.CreateShortRangeTower();
			} );
		l_userInput->RegisterClickAction( cuT( "Build2ThumbFrontButton" )
			, [this]()
			{
				m_game.CreateLongRangeTower();
			} );
		m_scene.GetEngine()->PostEvent( MakeFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.GetEngine()->GetOverlayCache();
				l_cache.Find( cuT( "HUDBuild1" ) )->SetVisible( true );
				l_cache.Find( cuT( "HUDBuild2" ) )->SetVisible( true );
			} ) );
	}

	void Hud::HideBuild()
	{
		m_scene.GetEngine()->PostEvent( MakeFunctorEvent( EventType::ePreRender
			, [this]()
			{
				auto & l_cache = m_scene.GetEngine()->GetOverlayCache();
				l_cache.Find( cuT( "HUDBuild1" ) )->SetVisible( false );
				l_cache.Find( cuT( "HUDBuild2" ) )->SetVisible( false );
			} ) );
		auto & l_userInput = m_scene.GetEngine()->GetUserInputListener();
		l_userInput->UnregisterClickAction( cuT( "Build1ThumbFrontButton" ) );
		l_userInput->UnregisterClickAction( cuT( "Build2ThumbFrontButton" ) );
	}

	void Hud::UpdateTowerInfo( Ecs const & p_ecs
		, Entity const & p_entity )
	{
		if ( p_entity )
		{
			m_scene.GetEngine()->PostEvent( MakeFunctorEvent( EventType::ePreRender
				, [this, &p_ecs, p_entity]()
				{
					DoUpdateText< uint32_t >( p_ecs, p_entity, Ecs::DamageComponent, m_towerDamage );
					DoUpdateText< Milliseconds >( p_ecs, p_entity, Ecs::CooldownComponent, m_towerSpeed );
					DoUpdateText< float >( p_ecs, p_entity, Ecs::RangeComponent, m_towerRange );
				} ) );
		}
		else
		{
			m_scene.GetEngine()->PostEvent( MakeFunctorEvent( EventType::ePreRender
				, [this, &p_ecs, p_entity]()
				{
					m_towerDamage.lock()->SetVisible( false );
					m_towerSpeed.lock()->SetVisible( false );
					m_towerRange.lock()->SetVisible( false );
				} ) );
		}
	}

	void Hud::Update()
	{
		//auto l_text = m_lives.lock();

		//if ( l_text )
		//{
		//	l_text->SetCaption( StringStream() << m_game.GetLives() );
		//}

		//l_text = m_ore.lock();

		//if ( l_text )
		//{
		//	l_text->SetCaption( StringStream() << m_game.GetOre() );
		//}

		//l_text = m_level.lock();

		//if ( l_text )
		//{
		//	l_text->SetCaption( StringStream() << m_game.GetWave() );
		//}

		//l_text = m_kills.lock();

		//if ( l_text )
		//{
		//	l_text->SetCaption( StringStream() << m_game.GetKills() );
		//}

		//l_text = m_enemyLife.lock();

		//if ( l_text )
		//{
		//	l_text->SetCaption( StringStream() << m_game.GetEnemiesLife() );
		//}

		//l_text = m_enemyBounty.lock();

		//if ( l_text )
		//{
		//	l_text->SetCaption( StringStream() << m_game.GetEnemiesBounty() );
		//}
	}
}
