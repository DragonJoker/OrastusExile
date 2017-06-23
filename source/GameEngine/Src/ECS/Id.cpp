#include "Id.hpp"

namespace orastus
{
	Id Id::Generate()
	{
		static uint64_t s_lastId = 0;
		Id l_result;
		l_result.m_id = ++s_lastId;
		return l_result;
	}

	bool operator==( Id const & p_lhs, Id const & p_rhs )
	{
		return p_lhs.m_id == p_rhs.m_id;
	}

	bool operator<( Id const & p_lhs, Id const & p_rhs )
	{
		return p_lhs.m_id < p_rhs.m_id;
	}

	std::ostream & operator<<( std::ostream & p_stream, Id const & p_value )
	{
		p_stream << p_value.m_id;
		return p_stream;
	}
}
