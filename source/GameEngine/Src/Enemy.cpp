#include "Enemy.hpp"

#include "Game.hpp"

namespace orastus
{
	Enemy::Enemy( Game & p_game )
		: m_game{ p_game }
		, m_speed{ p_game.GetComponent( Game::SpeedComponent ) }
		, m_life{ p_game.GetComponent( Game::LifeComponent ) }
		, m_state{ p_game.GetComponent( Game::StateComponent ) }
	{
	}

	void Enemy::CreateData( Entity const & p_entity
		, float p_speed
		, uint32_t p_life )
	{
		m_game.CreateComponentData( p_entity, m_speed, p_speed );
		m_game.CreateComponentData( p_entity, m_life, p_life );
		m_game.CreateComponentData( p_entity, m_state, EnemyState::eWalking );
	}
}
