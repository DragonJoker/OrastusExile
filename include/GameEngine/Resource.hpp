/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Resource_HPP___
#define ___EFO_Resource_HPP___

#include "GameEnginePrerequisites.hpp"

#include <cassert>
#include <algorithm>

namespace orastus
{
	/**
	*\brief
	*	A resource in the game.
	*/
	class Resource
	{
	public:
		/**
		*\brief
		*	Resource types enumeration.
		*/
		enum class Kind
		{
			eCloth,
			eLeather,
			eWood,
			eIron,
			eGold,
		};
		/**
		*\brief
		*	Exception thrown when trying to apply logic or arithmetic
		*	operations on resources of different kind.
		*/
		class KindException
			: public std::exception
		{
		public:
			/**
			*\brief
			*	Constructor, builds the message.
			*\param[in] expected
			*	The expected resource kind.
			*\param[in] received
			*	The received resource kind.
			*/
			KindException( Kind expected, Kind received )
				: std::exception{}
				, m_message{ "Expected [" + Resource::getKindName( expected ) + "], received [" + Resource::getKindName( received ) + "]" }
			{
			}
			/**
			*\return
			*	The exception message.
			*/
			inline char const * what()const override
			{
				return m_message.c_str();
			}

		private:
			std::string m_message;
		};

	public:
		/**
		*\name Defaulted copy/move ctors/assignment operators.
		*/
		/**@{*/
		Resource( Resource const & ) = default;
		Resource( Resource && ) = default;
		Resource & operator=( Resource const & ) = default;
		Resource & operator=( Resource && rhs ) = default;
		/**@}*/
		/**
		*\brief
		*	Constructor.
		*\param[in] kind
		*	The resource kind.
		*\param[in] count
		*	The resource count.
		*/
		inline Resource( Kind kind, uint32_t count )
			: m_kind{ kind }
			, m_count{ count }
		{
		}
		/**
		*\return
		*	The resource kind.
		*/
		inline Kind getKind()const
		{
			return m_kind;
		}
		/**
		*\return
		*	The resource count.
		*/
		inline uint32_t getCount()const
		{
			return m_count;
		}
		/**
		*\name Arithmetic member operators.
		*/
		/**@{*/
		inline Resource & operator+=( uint32_t rhs )
		{
			m_count += rhs;
			return *this;
		}

		inline Resource & operator-=( uint32_t rhs )
		{
			m_count -= std::min( m_count, rhs );
			return *this;
		}

		inline Resource & operator+=( Resource const & rhs )
		{
			if ( rhs.m_kind != m_kind )
			{
				throw KindException( m_kind, rhs.m_kind );
			}

			return operator+=( rhs.m_count );
		}

		inline Resource & operator-=( Resource const & rhs )
		{
			if ( rhs.m_kind != m_kind )
			{
				throw KindException( m_kind, rhs.m_kind );
			}

			return operator-=( rhs.m_count );
		}
		/**@}*/
		/**
		*\brief
		*	The name of given resource kind.
		*\param[in] kind
		*	The resource kind.
		*\return
		*	The resource kind name.
		*/
		static inline String getKindName( Kind kind )
		{
			switch ( kind )
			{
			case Kind::eCloth:
				return "Cloth";
			case Kind::eLeather:
				return "Leather";
			case Kind::eWood:
				return "Wood";
			case Kind::eIron:
				return "Iron";
			case Kind::eGold:
				return "Gold";
			default:
				assert( false );
				return "Undefined";
			}
		}

	private:
		Kind m_kind;
		uint32_t m_count;
	};
	/**
	*\name Arithmetic operators.
	*/
	/**@{*/
	inline Resource operator+( Resource const & lhs, uint32_t rhs )
	{
		Resource result{ lhs };
		result += rhs;
		return result;
	}

	inline Resource operator-( Resource const & lhs, uint32_t rhs )
	{
		Resource result{ lhs };
		result -= rhs;
		return result;
	}

	inline Resource operator+( Resource const & lhs, Resource const & rhs )
	{
		Resource result{ lhs };
		result += rhs;
		return result;
	}

	inline Resource operator-( Resource const & lhs, Resource const & rhs )
	{
		Resource result{ lhs };
		result -= rhs;
		return result;
	}
	/**@}*/
}

#endif
