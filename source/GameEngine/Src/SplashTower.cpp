#include "SplashTower.hpp"

#include "Game.hpp"

namespace orastus
{
	SplashTower::SplashTower( Game & p_game )
		: m_game{ p_game }
		, m_cooldown{ p_game.GetComponent( Game::CooldownComponent ) }
		, m_damage{ p_game.GetComponent( Game::DamageComponent ) }
		, m_range{ p_game.GetComponent( Game::RangeComponent ) }
		, m_bulletSpeed{ p_game.GetComponent( Game::SpeedComponent ) }
		, m_requiredLevel{ p_game.GetComponent( Game::LevelComponent ) }
		, m_state{ p_game.GetComponent( Game::StateComponent ) }
		, m_splashDamage{ p_game.GetComponent( Game::SplashDamageComponent ) }
		, m_splashRange{ p_game.GetComponent( Game::SplashRangeComponent ) }
	{
	}

	void SplashTower::CreateData( Entity const & p_entity
		, Milliseconds const & p_cooldown
		, uint32_t p_damage
		, float p_range
		, float p_bulletSpeed
		, uint32_t p_splashDamage
		, float p_splashRange
		, uint32_t p_requiredLevel )
	{
		m_game.CreateComponentData( p_entity, m_cooldown, p_cooldown );
		m_game.CreateComponentData( p_entity, m_damage, p_damage );
		m_game.CreateComponentData( p_entity, m_range, p_range );
		m_game.CreateComponentData( p_entity, m_bulletSpeed, p_bulletSpeed );
		m_game.CreateComponentData( p_entity, m_requiredLevel, p_requiredLevel );
		m_game.CreateComponentData( p_entity, m_splashDamage, p_splashDamage );
		m_game.CreateComponentData( p_entity, m_splashRange, p_splashRange );
		m_game.CreateComponentData( p_entity, m_state, TowerState::eIdle );
	}
}
