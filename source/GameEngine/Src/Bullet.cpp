#include "Bullet.hpp"

#include "Game.hpp"

namespace orastus
{
	Bullet::Bullet( Game & p_game )
		: m_game{ p_game }
		, m_speed{ p_game.GetComponent( Game::SpeedComponent ) }
		, m_damage{ p_game.GetComponent( Game::DamageComponent ) }
	{
	}

	void Bullet::CreateData( Entity const & p_entity
		, float p_speed
		, uint32_t p_damage )
	{
		m_game.CreateComponentData( p_entity, m_speed, p_speed );
		m_game.CreateComponentData( p_entity, m_damage, p_damage );
	}
}
