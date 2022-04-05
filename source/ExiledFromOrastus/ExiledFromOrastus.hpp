/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_ExiledFromOrastus_HPP___
#define ___EFO_ExiledFromOrastus_HPP___

#include "ExiledFromOrastusPrerequisites.hpp"

#pragma warning( push )
#pragma warning( disable: 4365 )
#pragma warning( disable: 4371 )
#pragma warning( disable: 5054 )
#include <wx/app.h>
#pragma warning( pop )

namespace orastus
{
	namespace main
	{
		class MainFrame;

		class ExiledFromOrastus
			: public wxApp
		{
		public:
			ExiledFromOrastus();

			inline MainFrame * getMainFrame()const
			{
				return m_mainFrame;
			}

			inline castor3d::Engine const & getCastor()const
			{
				return *m_castor;
			}

			inline castor3d::Engine & getCastor()
			{
				return *m_castor;
			}

		private:
			/**
			*\brief
			*	Initialises the application.
			*\remarks
			*	Parses the command line, loads language file, loads the plug-ins.
			*\return
			*	If false, the application will stop.
			*/
			bool OnInit()override;
			/**
			*\brief
			*	Cleans up the application.
			*/
			int OnExit()override;

		private:
			bool doParseCommandLine();
			bool doInitialiseLocale();
			bool doInitialiseCastor();
			void doLoadPlugins();
			void doCleanup();
			void doCleanupCastor();
			void doLoadImages();
			wxWindow * doInitialiseMainFrame();

		private:
			Config m_config;
			MainFrame * m_mainFrame{ nullptr };
			std::unique_ptr< castor3d::Engine > m_castor;
			castor::String m_fileName;
			castor::String m_rendererType{ cuT( "vulkan" ) };
			std::unique_ptr< wxLocale > m_locale;
		};
	}
}

wxDECLARE_APP( orastus::main::ExiledFromOrastus );

#endif
