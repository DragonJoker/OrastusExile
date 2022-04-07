#include "GameEngine/ECS/Id.hpp"

namespace orastus
{
	Id Id::generate()
	{
		static uint64_t lastId = 0;

		Id result;
		result.m_id = ++lastId;
		return result;
	}

	bool operator==( Id const & lhs, Id const & rhs )
	{
		return lhs.m_id == rhs.m_id;
	}

	bool operator<( Id const & lhs, Id const & rhs )
	{
		return lhs.m_id < rhs.m_id;
	}

	std::ostream & operator<<( std::ostream & stream, Id const & value )
	{
		stream << value.m_id;
		return stream;
	}
}
