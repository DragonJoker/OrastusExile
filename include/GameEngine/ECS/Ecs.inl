namespace orastus
{
	template< typename T >
	inline void Ecs::createComponentData( Entity const & p_entity
		, Component const & p_component
		, T const & p_data )
	{
		auto & l_datas = m_entitiesComponents.find( p_component.getName() )->second;
		l_datas.emplace_back( p_entity
			, makeComponentData( p_data ) );
	}

	template< typename T >
	inline ComponentData< T > const & Ecs::getComponentData( Entity const & p_entity
		, Component const & p_component )const
	{
		auto & l_datas = m_entitiesComponents.find( p_component.getName() )->second;
		auto l_it = std::find_if( std::begin( l_datas )
			, std::end( l_datas )
			, [&p_entity]( EntityComponentData const & p_data )
		{
			return p_data.entity == p_entity;
		} );

		if ( l_it == std::end( l_datas ) )
		{
			throw ComponentDataMatchException{ p_entity.getId(), p_component.getId() };
		}

		return componentCast< T >( *l_it->data );
	}

	template< typename T >
	inline ComponentData< T > & Ecs::getComponentData( Entity const & p_entity
		, Component const & p_component )
	{
		auto & l_datas = m_entitiesComponents.find( p_component.getName() )->second;
		auto l_it = std::find_if( std::begin( l_datas )
			, std::end( l_datas )
			, [&p_entity]( EntityComponentData & p_data )
		{
			return p_data.entity == p_entity;
		} );

		if ( l_it == std::end( l_datas ) )
		{
			throw ComponentDataMatchException{ p_entity.getId(), p_component.getId() };
		}

		return componentCast< T >( *l_it->data );
	}

	template< typename T >
	inline Entity Ecs::findEntity( Component const & p_component
		, T const & p_data )const
	{
		auto & l_datas = m_entitiesComponents.find( p_component.getName() )->second;
		auto l_it = std::find_if( std::begin( l_datas )
			, std::end( l_datas )
			, [&p_data]( EntityComponentData const & p_compData )
		{
			return componentCast< T >( *p_compData.data ).getValue() == p_data;
		} );

		Entity l_result;

		if ( l_it != std::end( l_datas ) )
		{
			l_result = l_it->entity;
		}

		return l_result;
	}

	inline ComponentId Ecs::hash( std::array< char, 8 > const & p_value )
	{
		ComponentId l_result{ 0u };

		for ( auto & l_char : p_value )
		{
			l_result <<= 8;
			l_result += l_char;
		}

		return l_result;
	}

	template< size_t N >
	inline ComponentId Ecs::hash( char const ( & p_value )[N] )
	{
		static_assert( N == 9, "A component name should only contain 8 characters" );
		ComponentId l_result{ 0u };

		for ( size_t i{ 0u }; i < 8; ++i )
		{
			l_result <<= 8;
			l_result += p_value[i];
		}

		return l_result;
	}

	inline String Ecs::unhash( ComponentId p_value )
	{
		String l_result{ "        " };

		for ( size_t i{ 0u }; i < 8; ++i )
		{
			l_result[7 - i] = char( p_value );
			p_value >>= 8;
		}

		return l_result;
	}
}
