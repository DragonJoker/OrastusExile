/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Id_HPP___
#define ___EFO_Id_HPP___

#include "GameEngine/GameEnginePrerequisites.hpp"

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
	*\param[in] lhs, rhs
	*	The operands.
	*\return
	*	\p false if \p lhs and \p rhs are different.
	*/
	EFO_API bool operator==( Id const & lhs, Id const & rhs );
	/**
	*\brief
	*	Equality operator.
	*\param[in] lhs, rhs
	*	The operands.
	*\return
	*	\p false if \p lhs and \p rhs are different.
	*/
	EFO_API bool operator<( Id const & lhs, Id const & rhs );
	/**
	*\brief
	*	Stream operator.
	*\param[in] stream
	*	The stream.
	*\param[in] value
	*	The ID.
	*\return
	*	The stream.
	*/
	EFO_API std::ostream & operator<<( std::ostream & stream, Id const & value );
	/**
	*\brief
	*	Gets an ID as text.
	*\param[in] id
	*	The ID.
	*\return
	*	The ID as text.
	*/
	inline String toString( Id const & id )
	{
		std::stringstream stream;
		stream << id;
		return stream.str();
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
	*\param[in] lhs, rhs
	*	The operands.
	*\return
	*	\p false if \p lhs and \p rhs are different.
	*/
	bool inline operator==( Ided const & lhs, Ided const & rhs )
	{
		return lhs.m_id == rhs.m_id;
	}
}

#endif
