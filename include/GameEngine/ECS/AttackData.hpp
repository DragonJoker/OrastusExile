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
		inline AttackData( Milliseconds const & pshootDelay )
			: shootDelay{ pshootDelay }
		{
		}

		Milliseconds shootDelay;
		Entity target;
		bool shot{ false };
	};
}

#endif
