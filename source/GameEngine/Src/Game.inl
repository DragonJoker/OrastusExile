namespace orastus
{
	template< typename T >
	inline void Game::CreateComponentData( Entity const & p_entity
		, Component const & p_component
		, T const & p_data )
	{
		m_entitiesComponents.emplace_back( p_entity
			, p_component
			, MakeComponentData( p_data ) );
	}

	template< typename T >
	inline ComponentData< T > const & Game::GetComponentData( Entity const & p_entity
		, Component const & p_component )const
	{
		auto l_it = std::find_if( std::begin( m_entitiesComponents )
			, std::end( m_entitiesComponents )
			, [&p_entity, &p_component]( EntityComponentData const & p_data )
		{
			return p_data.m_entity == p_entity.GetId()
				&& p_data.m_component == p_component.GetId();
		} );

		if ( l_it == std::end( m_entitiesComponents ) )
		{
			throw ComponentDataMatchException{ p_entity.GetId(), p_component.GetId() };
		}

#if !defined( NDEBUG )

		auto l_typed = dynamic_cast< ComponentData< T > const * >( l_it->m_data.get() );

		if ( !l_typed )
		{
			FAILURE( "ComponentData type mismatch." );
		}

#endif

		return static_cast< ComponentData< T > const & >( *l_it->m_data );
	}
}
