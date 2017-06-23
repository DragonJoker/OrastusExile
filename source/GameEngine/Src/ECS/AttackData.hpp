/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_AttackData_HPP___
#define ___EFO_AttackData_HPP___

#include "Entity.hpp"

namespace orastus
{
	struct AttackData
	{
		inline AttackData( Milliseconds const & p_shootDelay )
			: m_shootDelay{ p_shootDelay }
		{
		}

		Milliseconds m_shootDelay;
		Entity m_target;
		bool m_shot{ false };
	};
}

#endif
