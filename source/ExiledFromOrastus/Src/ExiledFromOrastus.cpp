#include "ExiledFromOrastus.hpp"

#include "ImagesLoader.hpp"
#include "MainFrame.hpp"

#include <Log/Logger.hpp>
#include <Engine.hpp>
#include <Plugin/RendererPlugin.hpp>

#include <wx/cmdline.h>
#include <wx/display.h>

//wxIMPLEMENT_APP( orastus:main::ExiledFromOrastus );

using namespace Castor;
using namespace Castor3D;

#if defined( NDEBUG )

static const LogType ELogType_DEFAULT = LogType::eInfo;

#else

static const LogType ELogType_DEFAULT = LogType::eDebug;

#endif

namespace orastus
{
	namespace main
	{
		static wxString const ShortName = wxT( "ExiledFromOrastus" );
		static wxString const LongName = wxT( "Exiled From Orastus" );

		ExiledFromOrastus::ExiledFromOrastus()
			: wxApp{}
		{
		}

		bool ExiledFromOrastus::OnInit()
		{
#if defined( CASTOR_PLATFORM_WINDOWS ) && !defined( NDEBUG ) && !defined( VLD_AVAILABLE )

			_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

#endif

			bool l_return = DoParseCommandLine();
			wxDisplay l_display;
			wxRect l_rect = l_display.GetClientArea();
			wxWindow * l_window = nullptr;

			if ( l_return )
			{
				l_return = DoInitialiseLocale();
			}

			if ( l_return )
			{
				try
				{
					DoLoadImages();
					l_return = DoInitialiseCastor();

					if ( l_return )
					{
						l_window = DoInitialiseMainFrame();
						l_return = l_window != nullptr;
					}
				}
				catch ( Exception & exc )
				{
					Logger::LogError( std::stringstream() << ShortName << " - Initialisation failed : " << exc.GetFullDescription() );
					l_return = false;
				}
				catch ( std::exception & exc )
				{
					Logger::LogError( std::stringstream() << ShortName << " - Initialisation failed : " << exc.what() );
					l_return = false;
				}
			}

			wxApp::SetTopWindow( l_window );

			if ( !l_return )
			{
				DoCleanup();
			}

			return l_return;
		}

		int ExiledFromOrastus::OnExit()
		{
			Logger::LogInfo( ShortName.ToStdString() + cuT( " - Exit" ) );
			DoCleanup();
			return wxApp::OnExit();
		}

		bool ExiledFromOrastus::DoParseCommandLine()
		{
			wxCmdLineParser l_parser( wxApp::argc, wxApp::argv );
			l_parser.AddSwitch( wxT( "h" ), wxT( "help" ), _( "Displays this help" ) );
			l_parser.AddOption( wxT( "l" ), wxT( "log" ), _( "Defines log level" ), wxCMD_LINE_VAL_NUMBER );
			l_parser.AddParam( _( "The initial scene file" ), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL );
			l_parser.AddSwitch( wxT( "opengl" ), wxEmptyString, _( "Defines the renderer to OpenGl" ) );
			l_parser.AddSwitch( wxT( "test" ), wxEmptyString, _( "Defines the renderer to Test" ) );
			bool l_return = l_parser.Parse( false ) == 0;

			// S'il y avait des erreurs ou "-h" ou "--help", on affiche l'aide et on sort
			if ( !l_return || l_parser.Found( wxT( 'h' ) ) )
			{
				l_parser.Usage();
				l_return = false;
			}

			if ( l_return )
			{
				LogType l_eLogLevel = LogType::eCount;
				long l_log;

				if ( !l_parser.Found( wxT( "l" ), &l_log ) )
				{
					l_eLogLevel = ELogType_DEFAULT;
				}
				else
				{
					l_eLogLevel = LogType( l_log );
				}

				Logger::Initialise( l_eLogLevel );

				if ( l_parser.Found( wxT( "opengl" ) ) )
				{
					m_rendererType = cuT( "opengl" );
				}

				if ( l_parser.Found( wxT( "test" ) ) )
				{
					m_rendererType = cuT( "test" );
				}

				wxString l_strFileName;

				if ( l_parser.GetParamCount() > 0 )
				{
					m_fileName = l_parser.GetParam( 0 ).ToStdString();
				}
			}

			return l_return;
		}

		bool ExiledFromOrastus::DoInitialiseLocale()
		{
			long l_lLanguage = wxLANGUAGE_DEFAULT;
			Path l_pathCurrent = File::GetExecutableDirectory().GetPath();

			// load language if possible, fall back to english otherwise
			if ( wxLocale::IsAvailable( l_lLanguage ) )
			{
				m_locale = std::make_unique< wxLocale >( l_lLanguage, wxLOCALE_LOAD_DEFAULT );
				// add locale search paths
				m_locale->AddCatalogLookupPathPrefix( l_pathCurrent / cuT( "share" ) / ShortName.ToStdString() );
				m_locale->AddCatalog( ShortName );

				if ( !m_locale->IsOk() )
				{
					std::cerr << "Selected language is wrong" << std::endl;
					l_lLanguage = wxLANGUAGE_ENGLISH;
					m_locale = std::make_unique< wxLocale >( l_lLanguage );
				}
			}
			else
			{
				std::cerr << "The selected language is not supported by your system."
					<< "Try installing support for this language." << std::endl;
				l_lLanguage = wxLANGUAGE_ENGLISH;
				m_locale = std::make_unique< wxLocale >( l_lLanguage );
			}

			return true;
		}

		bool ExiledFromOrastus::DoInitialiseCastor()
		{
			bool l_return = true;

			if ( !File::DirectoryExists( Engine::GetEngineDirectory() ) )
			{
				File::DirectoryCreate( Engine::GetEngineDirectory() );
			}

			Logger::SetFileName( Engine::GetEngineDirectory() / ( ShortName.ToStdString() + cuT( ".log" ) ) );
			Logger::LogInfo( ShortName.ToStdString() + cuT( " - Start" ) );

			m_castor = std::make_unique< Engine >();
			DoLoadPlugins();

			auto l_renderers = m_castor->GetPluginCache().GetPlugins( PluginType::eRenderer );

			if ( l_renderers.empty() )
			{
				CASTOR_EXCEPTION( "No renderer plug-ins" );
			}
			else if ( l_renderers.size() == 1 )
			{
				m_rendererType = std::static_pointer_cast< RendererPlugin >( l_renderers.begin()->second )->GetRendererType();
			}

			l_return = true;

			if ( l_return )
			{
				l_return = m_castor->LoadRenderer( m_rendererType );
			}

			return l_return;
		}

		void ExiledFromOrastus::DoLoadPlugins()
		{
			PathArray l_arrayFiles;
			File::ListDirectoryFiles( Engine::GetPluginsDirectory(), l_arrayFiles );
			PathArray l_arrayKept;

			// Exclude debug plug-in in release builds, and release plug-ins in debug builds
			for ( auto l_file : l_arrayFiles )
			{
#if defined( NDEBUG )

				if ( l_file.find( String( cuT( "d." ) ) + CASTOR_DLL_EXT ) == String::npos )
#else

				if ( l_file.find( String( cuT( "d." ) ) + CASTOR_DLL_EXT ) != String::npos )

#endif
				{
					l_arrayKept.push_back( l_file );
				}
			}

			if ( !l_arrayKept.empty() )
			{
				PathArray l_arrayFailed;
				PathArray l_otherPlugins;

				for ( auto l_file : l_arrayKept )
				{
					if ( l_file.GetExtension() == CASTOR_DLL_EXT )
					{
						// Since techniques depend on renderers, we load these first
						if ( l_file.find( cuT( "RenderSystem" ) ) != String::npos )
						{
							if ( !m_castor->GetPluginCache().LoadPlugin( l_file ) )
							{
								l_arrayFailed.push_back( l_file );
							}
						}
						else
						{
							l_otherPlugins.push_back( l_file );
						}
					}
				}

				// Then we load other plug-ins
				for ( auto l_file : l_otherPlugins )
				{
					if ( !m_castor->GetPluginCache().LoadPlugin( l_file ) )
					{
						l_arrayFailed.push_back( l_file );
					}
				}

				if ( !l_arrayFailed.empty() )
				{
					Logger::LogWarning( cuT( "Some plug-ins couldn't be loaded :" ) );

					for ( auto l_file : l_arrayFailed )
					{
						Logger::LogWarning( Path( l_file ).GetFileName() );
					}

					l_arrayFailed.clear();
				}
			}

			Logger::LogInfo( cuT( "Plugins loaded" ) );
		}

		void ExiledFromOrastus::DoLoadImages()
		{
		}

		wxWindow * ExiledFromOrastus::DoInitialiseMainFrame()
		{
			wxAppConsole::SetAppName( ShortName );
			wxAppConsole::SetVendorName( wxT( "dragonjoker" ) );

#if wxCHECK_VERSION( 2, 9, 0 )

			wxAppConsole::SetAppDisplayName( LongName );
			wxAppConsole::SetVendorDisplayName( wxT( "DragonJoker" ) );

#endif

			m_mainFrame = new MainFrame;

			SetTopWindow( m_mainFrame );
			return m_mainFrame;
		}

		void ExiledFromOrastus::DoCleanup()
		{
			DoCleanupCastor();
			m_locale.reset();
			ImagesLoader::Cleanup();
			Logger::Cleanup();
			wxImage::CleanUpHandlers();
		}

		void ExiledFromOrastus::DoCleanupCastor()
		{
			m_castor.reset();
		}
	}
}
