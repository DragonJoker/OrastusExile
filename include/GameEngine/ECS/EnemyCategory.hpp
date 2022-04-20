/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_EnemyCategory_HPP___
#define ___EFO_EnemyCategory_HPP___

#include "GameEngine/Ability.hpp"

namespace orastus
{
	struct EnemyCategory
	{
		EnemyCategory( float speed
			, Ability< uint32_t > life
			, Ability< uint32_t > bounty )
			: m_speed{ speed }
			, m_life{ std::move( life ) }
			, m_bounty{ std::move( bounty ) }
		{
		}

		EFO_API virtual ~EnemyCategory() = default;

		void upgrade()
		{
			m_life.upgrade();
			m_bounty.upgrade();
		}

		float getSpeed()const
		{
			return m_speed;
		}

		uint32_t getLife()const
		{
			return m_life.getValue();
		}

		uint32_t getBounty()const
		{
			return m_bounty.getValue();
		}

	private:
		float m_speed;
		Ability< uint32_t > m_life;
		Ability< uint32_t > m_bounty;
	};

	struct BaseEnemy
		: public EnemyCategory
	{
		EFO_API BaseEnemy();
	};
}

#endif
