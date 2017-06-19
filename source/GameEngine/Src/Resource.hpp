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
	class Resource
	{
	public:
		enum class Kind
		{
			eCloth,
			eLeather,
			eWood,
			eIron,
			eGold,
		};

		class KindException
			: public std::exception
		{
		public:
			KindException( Kind p_expected, Kind p_received )
				: std::exception{}
				, m_message{ "Expected [" + Resource::GetKindName( p_expected ) + "], received [" + Resource::GetKindName( p_received ) + "]" }
			{
			}

			inline char const * what()const override
			{
				return m_message.c_str();
			}

		private:
			std::string m_message;
		};

	public:
		Resource( Resource const & ) = default;
		Resource( Resource && ) = default;
		Resource & operator=( Resource const & ) = default;
		Resource & operator=( Resource && p_rhs ) = default;

		inline Resource( Kind p_kind, uint32_t p_count )
			: m_kind{ p_kind }
			, m_count{ p_count }
		{
		}

		inline Kind GetKind()const
		{
			return m_kind;
		}

		inline uint32_t GetCount()const
		{
			return m_count;
		}

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
}

#endif
