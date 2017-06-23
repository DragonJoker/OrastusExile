namespace orastus
{
	template< typename T >
	inline void Ecs::CreateComponentData( Entity const & p_entity
		, Component const & p_component
		, T const & p_data )
	{
		auto & l_datas = m_entitiesComponents.find( p_component.GetName() )->second;
		l_datas.emplace_back( p_entity
			, MakeComponentData( p_data ) );
	}

	template< typename T >
	inline ComponentData< T > const & Ecs::GetComponentData( Entity const & p_entity
		, Component const & p_component )const
	{
		auto & l_datas = m_entitiesComponents.find( p_component.GetName() )->second;
		auto l_it = std::find_if( std::begin( l_datas )
			, std::end( l_datas )
			, [&p_entity, &p_component]( EntityComponentData const & p_data )
		{
			return p_data.m_entity == p_entity;
		} );

		if ( l_it == std::end( l_datas ) )
		{
			throw ComponentDataMatchException{ p_entity.GetId(), p_component.GetId() };
		}

		return ComponentCast< T >( *l_it->m_data );
	}

	template< typename T >
	inline ComponentData< T > & Ecs::GetComponentData( Entity const & p_entity
		, Component const & p_component )
	{
		auto & l_datas = m_entitiesComponents.find( p_component.GetName() )->second;
		auto l_it = std::find_if( std::begin( l_datas )
			, std::end( l_datas )
			, [&p_entity, &p_component]( EntityComponentData & p_data )
		{
			return p_data.m_entity == p_entity;
		} );

		if ( l_it == std::end( l_datas ) )
		{
			throw ComponentDataMatchException{ p_entity.GetId(), p_component.GetId() };
		}

		return ComponentCast< T >( *l_it->m_data );
	}

	template< typename T >
	inline Entity Ecs::FindEntity( Component const & p_component
		, T const & p_data )const
	{
		auto & l_datas = m_entitiesComponents.find( p_component.GetName() )->second;
		auto l_it = std::find_if( std::begin( l_datas )
			, std::end( l_datas )
			, [&p_data, &p_component]( EntityComponentData const & p_compData )
		{
			return ComponentCast< T >( *p_compData.m_data ).GetValue() == p_data;
		} );

		Entity l_result;

		if ( l_it != std::end( l_datas ) )
		{
			l_result = l_it->m_entity;
		}

		return l_result;
	}

	inline ComponentId Ecs::Hash( std::array< char, 8 > const & p_value )
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
	inline ComponentId Ecs::Hash( char const ( & p_value )[N] )
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

	inline String Ecs::Unhash( ComponentId p_value )
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
