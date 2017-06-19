/**
See licence file in root folder, MIT.txt
*/
#pragma once
#ifndef ___EFO_ExiledFromOrastus_HPP___
#define ___EFO_ExiledFromOrastus_HPP___

#include "ExiledFromOrastusPrerequisites.hpp"

#include <wx/app.h>

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

			inline MainFrame * GetMainFrame()const
			{
				return m_mainFrame;
			}

			inline Castor3D::Engine const & GetCastor()const
			{
				return *m_castor;
			}

			inline Castor3D::Engine & GetCastor()
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
			virtual bool OnInit();
			/**
			*\brief
			*	Cleans up the application.
			*/
			virtual int OnExit();

		private:
			bool DoParseCommandLine();
			bool DoInitialiseLocale();
			bool DoInitialiseCastor();
			void DoLoadPlugins();
			void DoCleanup();
			void DoCleanupCastor();
			void DoLoadImages();
			wxWindow * DoInitialiseMainFrame();

		private:
			MainFrame * m_mainFrame{ nullptr };
			std::unique_ptr< Castor3D::Engine > m_castor;
			Castor::String m_fileName;
			Castor::String m_rendererType{ cuT( "opengl" ) };
			std::unique_ptr< wxLocale > m_locale;
		};
	}
}

wxDECLARE_APP( orastus::main::ExiledFromOrastus );

#endif
