/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Id_HPP___
#define ___EFO_Id_HPP___

#include "GameEnginePrerequisites.hpp"

#include <sstream>

namespace orastus
{
	struct NoInit
	{
	};
	/**
	*\brief
	*	An ID, for entities and components of an ECS.
	*/
	class Id
	{
	public:
		/**
		*\return
		*	A unique ID.
		*/
		EFO_API static Id generate();
		/**
		*\return
		*	\p true if the id is generated.
		*/
		inline explicit operator bool()const
		{
			return m_id != 0u;
		}
		/**
		*\return
		*	\p true if the id is generated.
		*/
		inline explicit operator uint64_t()const
		{
			return m_id;
		}

	private:
		uint64_t m_id{ 0u };
		friend EFO_API bool operator==( Id const &, Id const & );
		friend EFO_API bool operator<( Id const &, Id const & );
		friend EFO_API std::ostream & operator<<( std::ostream &, Id const & );
	};
	/**
	*\brief
	*	Equality operator.
	*\param[in] p_lhs, p_rhs
	*	The operands.
	*\return
	*	\p false if \p p_lhs and \p rhs are different.
	*/
	EFO_API bool operator==( Id const & p_lhs, Id const & p_rhs );
	/**
	*\brief
	*	Equality operator.
	*\param[in] p_lhs, p_rhs
	*	The operands.
	*\return
	*	\p false if \p p_lhs and \p rhs are different.
	*/
	EFO_API bool operator<( Id const & p_lhs, Id const & p_rhs );
	/**
	*\brief
	*	Stream operator.
	*\param[in] p_stream
	*	The stream.
	*\param[in] p_value
	*	The ID.
	*\return
	*	The stream.
	*/
	EFO_API std::ostream & operator<<( std::ostream & p_stream, Id const & p_value );
	/**
	*\brief
	*	Gets an ID as text.
	*\param[in] p_id
	*	The ID.
	*\return
	*	The ID as text.
	*/
	inline String toString( Id const & p_id )
	{
		std::stringstream l_stream;
		l_stream << p_id;
		return l_stream.str();
	}
	/**
	*\brief
	*	Base class for those which need an ID (component and entity, for instance).
	*/
	class Ided
	{
	protected:
		/**
		*\brief
		*	Constructor, does not generates the ID.
		*/
		inline explicit Ided( NoInit const & )
		{
		}

	public:
		/**
		*\brief
		*	Constructor, generates the ID.
		*/
		inline Ided()
			: m_id{ Id::generate() }
		{
		}
		/**
		*\return
		*	The ID.
		*/
		inline Id const & getId()const
		{
			return m_id;
		}
		/**
		*\return
		*	\p true if the id is generated.
		*/
		inline explicit operator bool()const
		{
			return static_cast< bool >( m_id );
		}

	private:
		Id m_id;
		friend bool operator==( Ided const &, Ided const & );
	};
	/**
	*\brief
	*	Equality operator.
	*\param[in] p_lhs, p_rhs
	*	The operands.
	*\return
	*	\p false if \p p_lhs and \p rhs are different.
	*/
	bool inline operator==( Ided const & p_lhs, Ided const & p_rhs )
	{
		return p_lhs.m_id == p_rhs.m_id;
	}
}

#endif
