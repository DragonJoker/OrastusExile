/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_Component_HPP___
#define ___EFO_Component_HPP___

#include "Id.hpp"

#include <Design/Named.hpp>

namespace orastus
{
	/**
	*\brief
	*	A component's description.
	*/
	class Component
		: public Ided
		, public Castor::Named
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
			*\param[in] p_name
			*	The name which is not found.
			*/
			NameException( String const & p_name )
				: std::exception{}
				, m_message{ "Unknown component name [" + p_name + "]" }
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
		*\param[in] p_name
		*	The internal component name.
		*\param[in] p_desc
		*	The human readable description.
		*/
		inline Component( String const & p_name
			, String const & p_desc )
			: Castor::Named{ p_name }
			, m_desc{ p_desc }
		{
		}
		/**
		*\brief
		*	The component's description.
		*/
		String const & GetDesc()const
		{
			return m_desc;
		}

	private:
		String m_desc;
	};
	//! A list of components.
	using ComponentList = std::vector< Component >;
}

#endif
