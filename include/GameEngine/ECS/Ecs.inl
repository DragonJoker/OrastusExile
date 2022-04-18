namespace orastus
{
	template< typename T >
	inline void Ecs::createComponentData( Entity const & entity
		, Component const & component
		, T data )
	{
		auto & datas = m_entitiesComponents.find( component.getName() )->second;
		datas.emplace_back( entity
			, makeComponentData( std::move( data ) ) );
	}

	template< typename T >
	inline ComponentData< T > const & Ecs::getComponentData( Entity const & entity
		, Component const & component )const
	{
		auto & datas = m_entitiesComponents.find( component.getName() )->second;
		auto it = std::find_if( std::begin( datas )
			, std::end( datas )
			, [&entity]( EntityComponentData const & lookup )
			{
				return lookup.entity == entity;
			} );

		if ( it == std::end( datas ) )
		{
			throw ComponentDataMatchException{ entity.getId(), component.getId() };
		}

		return componentCast< T >( *it->data );
	}

	template< typename T >
	inline ComponentData< T > & Ecs::getComponentData( Entity const & entity
		, Component const & component )
	{
		auto & datas = m_entitiesComponents.find( component.getName() )->second;
		auto it = std::find_if( std::begin( datas )
			, std::end( datas )
			, [&entity]( EntityComponentData & lookup )
			{
				return lookup.entity == entity;
			} );

		if ( it == std::end( datas ) )
		{
			throw ComponentDataMatchException{ entity.getId(), component.getId() };
		}

		return componentCast< T >( *it->data );
	}

	template< typename T >
	inline Entity Ecs::findEntity( Component const & component
		, T const & data )const
	{
		auto & datas = m_entitiesComponents.find( component.getName() )->second;
		auto it = std::find_if( std::begin( datas )
			, std::end( datas )
			, [&data]( EntityComponentData const & lookup )
			{
				return componentCast< T >( *lookup.data ).getValue() == data;
			} );

		Entity result;

		if ( it != std::end( datas ) )
		{
			result = it->entity;
		}

		return result;
	}

	inline ComponentId Ecs::hash( std::array< char, 8 > const & value )
	{
		ComponentId result{ 0u };

		for ( auto & c : value )
		{
			result <<= 8;
			result += c;
		}

		return result;
	}

	template< size_t N >
	inline ComponentId Ecs::hash( char const ( & value )[N] )
	{
		static_assert( N == 9, "A component name should only contain 8 characters" );
		ComponentId result{ 0u };

		for ( size_t i{ 0u }; i < 8; ++i )
		{
			result <<= 8;
			result += value[i];
		}

		return result;
	}

	inline String Ecs::unhash( ComponentId value )
	{
		String result{ "        " };

		for ( size_t i{ 0u }; i < 8; ++i )
		{
			result[7 - i] = char( value );
			value >>= 8;
		}

		return result;
	}
}
