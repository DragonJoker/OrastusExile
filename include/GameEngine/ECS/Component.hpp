/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Component_HPP___
#define ___EFO_Component_HPP___

#include "Id.hpp"

#include <CastorUtils/Design/Named.hpp>

namespace orastus
{
	/**
	*\brief
	*	A component's description.
	*/
	class Component
		: public Ided
	{
	public:
		/**
		*\brief
		*	The exception thrown when no component with given name is found.
		*/
		class NameException
			: public std::exception
		{
		public:
			/**
			*\brief
			*	Constructor, builds the message.
			*\param[in] name
			*	The name which is not found.
			*/
			explicit NameException( ComponentId const & name )
				: std::exception{}
				, m_message{ "Unknown component name [" + toString( name ) + "]" }
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
		*\brief
		*	Constructor.
		*\param[in] name
		*	The internal component name.
		*\param[in] desc
		*	The human readable description.
		*/
		inline explicit Component( ComponentId const & name
			, String const & desc )
			: m_name{ name }
			, m_desc{ desc }
		{
		}
		/**
		*\brief
		*	The component internal name.
		*/
		inline ComponentId const & getName()const
		{
			return m_name;
		}
		/**
		*\brief
		*	The component's description.
		*/
		inline String const & getDesc()const
		{
			return m_desc;
		}

	private:
		ComponentId m_name;
		String m_desc;
	};
	//! A list of components.
	using ComponentList = std::vector< Component >;
}

#endif
