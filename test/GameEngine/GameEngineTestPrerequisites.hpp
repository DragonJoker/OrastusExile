/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EOFT_GameEngineTestPrerequisites_HPP___
#define ___EOFT_GameEngineTestPrerequisites_HPP___

#include <CastorTest/UnitTest.hpp>

#include <ECS/Ecs.hpp>
#include <Resource.hpp>
#include <State/EnemyState.hpp>
#include <State/TowerState.hpp>

namespace Testing
{
	template<>
	struct Stringifier< orastus::Id >
	{
		static std::string get( orastus::Id const & value )
		{
			std::stringstream stream;
			stream << uint64_t( value );
			return stream.str();
		}
	};

	template<>
	struct Stringifier< orastus::EnemyState >
	{
		static std::string get( orastus::EnemyState const & value )
		{
			return orastus::getName( value );
		}
	};

	template<>
	struct Stringifier< orastus::TowerState >
	{
		static std::string get( orastus::TowerState const & value )
		{
			return orastus::getName( value );
		}
	};

	template<>
	struct Stringifier< orastus::Resource::Kind >
	{
		static std::string get( orastus::Resource::Kind const & value )
		{
			return orastus::Resource::getKindName( value );
		}
	};

	template< typename DataT >
	struct Stringifier< orastus::ComponentData< DataT > >
	{
		static std::string get( orastus::ComponentData< DataT > const & value )
		{
			return Stringifier< DataT >::get( value.getValue() );
		}
	};
}

#endif
