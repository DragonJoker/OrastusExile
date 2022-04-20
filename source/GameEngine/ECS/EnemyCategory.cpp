#include "GameEngine/ECS/EnemyCategory.hpp"

namespace orastus
{
	//*********************************************************************************************

	BaseEnemy::BaseEnemy()
		: EnemyCategory{ 40.0f
		, { 1u
			, []( uint32_t value, uint32_t level )
			{
				return value + std::max( level, uint32_t( value * 5.0 / 100.0 ) );
			} }
		, { 1u
			, []( uint32_t value, uint32_t level )
			{
				return value + std::max( 2u, ( value * 4 ) / 100 );
			} } }
	{
	}

	//*********************************************************************************************
}
