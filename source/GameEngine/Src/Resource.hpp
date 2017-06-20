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
			*\param[in] p_expected
			*	The expected resource kind.
			*\param[in] p_received
			*	The received resource kind.
			*/
			KindException( Kind p_expected, Kind p_received )
				: std::exception{}
				, m_message{ "Expected [" + Resource::GetKindName( p_expected ) + "], received [" + Resource::GetKindName( p_received ) + "]" }
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
		Resource & operator=( Resource && p_rhs ) = default;
		/**@}*/
		/**
		*\brief
		*	Constructor.
		*\param[in] p_kind
		*	The resource kind.
		*\param[in] p_count
		*	The resource count.
		*/
		inline Resource( Kind p_kind, uint32_t p_count )
			: m_kind{ p_kind }
			, m_count{ p_count }
		{
		}
		/**
		*\return
		*	The resource kind.
		*/
		inline Kind GetKind()const
		{
			return m_kind;
		}
		/**
		*\return
		*	The resource count.
		*/
		inline uint32_t GetCount()const
		{
			return m_count;
		}
		/**
		*\name Arithmetic member operators.
		*/
		/**@{*/
		inline Resource & operator+=( uint32_t p_rhs )
		{
			m_count += p_rhs;
			return *this;
		}

		inline Resource & operator-=( uint32_t p_rhs )
		{
			m_count -= std::min( m_count, p_rhs );
			return *this;
		}

		inline Resource & operator+=( Resource const & p_rhs )
		{
			if ( p_rhs.m_kind != m_kind )
			{
				throw KindException( m_kind, p_rhs.m_kind );
			}

			return operator+=( p_rhs.m_count );
		}

		inline Resource & operator-=( Resource const & p_rhs )
		{
			if ( p_rhs.m_kind != m_kind )
			{
				throw KindException( m_kind, p_rhs.m_kind );
			}

			return operator-=( p_rhs.m_count );
		}
		/**@}*/
		/**
		*\brief
		*	The name of given resource kind.
		*\param[in] p_kind
		*	The resource kind.
		*\return
		*	The resource kind name.
		*/
		static inline String GetKindName( Kind p_kind )
		{
			String l_result;

			switch ( p_kind )
			{
			case Kind::eCloth:
				l_result = "Cloth";
				break;

			case Kind::eLeather:
				l_result = "Leather";
				break;

			case Kind::eWood:
				l_result = "Wood";
				break;

			case Kind::eIron:
				l_result = "Iron";
				break;

			case Kind::eGold:
				l_result = "Gold";
				break;

			default:
				assert( false );
				l_result = "Undefined";
				break;
			}

			return l_result;
		}

	private:
		Kind m_kind;
		uint32_t m_count;
	};
	/**
	*\name Arithmetic operators.
	*/
	/**@{*/
	inline Resource operator+( Resource const & p_lhs, uint32_t p_rhs )
	{
		Resource l_result{ p_lhs };
		l_result += p_rhs;
		return l_result;
	}

	inline Resource operator-( Resource const & p_lhs, uint32_t p_rhs )
	{
		Resource l_result{ p_lhs };
		l_result -= p_rhs;
		return l_result;
	}

	inline Resource operator+( Resource const & p_lhs, Resource const & p_rhs )
	{
		Resource l_result{ p_lhs };
		l_result += p_rhs;
		return l_result;
	}

	inline Resource operator-( Resource const & p_lhs, Resource const & p_rhs )
	{
		Resource l_result{ p_lhs };
		l_result -= p_rhs;
		return l_result;
	}
	/**@}*/
}

#endif
