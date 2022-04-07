#include "OrastusExile.hpp"

#include "ImagesLoader.hpp"
#include "MainFrame.hpp"

#include <CastorUtils/Log/Logger.hpp>
#include <Castor3D/Engine.hpp>
#include <Castor3D/Render/RenderSystem.hpp>

#pragma warning( push )
#pragma warning( disable: 4251 )
#pragma warning( disable: 4365 )
#include <wx/cmdline.h>
#include <wx/display.h>
#include <wx/fileconf.h>
#pragma warning( pop )

wxIMPLEMENT_APP( orastus::main::OrastusExile );

using namespace castor;
using namespace castor3d;

namespace orastus
{
	namespace main
	{
		static constexpr uint32_t DefaultGpuIndex = 0u;
#if defined( NDEBUG )
		static constexpr castor::LogType DefaultLogType = castor::LogType::eInfo;
		static constexpr bool DefaultValidation = false;
#else
		static constexpr castor::LogType DefaultLogType = castor::LogType::eTrace;
		static constexpr bool DefaultValidation = true;
#endif

		namespace option
		{
			namespace lg
			{
				static const wxString Help{ wxT( "help" ) };
				static const wxString Config{ wxT( "config" ) };
				static const wxString LogLevel{ wxT( "log" ) };
				static const wxString Validate{ wxT( "validate" ) };
				static const wxString GpuIndex{ wxT( "gpu" ) };
			}

			namespace st
			{
				static const wxString Help{ wxT( "h" ) };
				static const wxString Config{ wxT( "c" ) };
				static const wxString LogLevel{ wxT( "l" ) };
				static const wxString Validate{ wxT( "a" ) };
				static const wxString GpuIndex{ wxT( "g" ) };
			}
		}

		static bool endsWith( castor::String const & value
			, castor::String const & lookup )
		{
			auto it = value.find( lookup );
			bool result = it != castor::String::npos;

			if ( result )
			{
				result = ( it + lookup.size() ) == value.size();
			}

			return result;
		}

		struct Options
		{
			Options( Options const & ) = delete;
			Options & operator=( Options const & ) = delete;
			Options( Options && ) = delete;
			Options & operator=( Options && ) = delete;

			Options( int argc, wxCmdLineArgsArray const & argv )
				: parser{ argc, argv }
			{
				static const wxString Help{ _( "Displays this help." ) };
				static const wxString Config{ _( "Specifies the configuration file." ) };
				static const wxString LogLevel{ _( "Defines log level (from 0=trace to 4=error)." ) };
				static const wxString Validate{ _( "Enables rendering API validation." ) };
				static const wxString GpuIndex{ _( "The index of the wanted Vulkan physical device." ) };

				parser.AddSwitch( option::st::Help, option::lg::Help, Help, wxCMD_LINE_OPTION_HELP );
				parser.AddOption( option::st::Config, option::lg::Config, Config, wxCMD_LINE_VAL_STRING, 0 );
				parser.AddOption( option::st::LogLevel, option::lg::LogLevel, LogLevel, wxCMD_LINE_VAL_NUMBER );
				parser.AddSwitch( option::st::Validate, option::lg::Validate, Validate );
				parser.AddOption( option::st::GpuIndex, option::lg::GpuIndex, GpuIndex, wxCMD_LINE_VAL_NUMBER );

				for ( auto & plugin : list )
				{
					auto desc = makeWxString( plugin.description );
					desc.Replace( wxT( " renderer for Ashes" ), wxEmptyString );
					parser.AddSwitch( makeWxString( plugin.name )
						, wxEmptyString
						, _( "Defines the renderer to " ) + desc + wxT( "." ) );
				}

				if ( ( parser.Parse( false ) != 0 )
					|| parser.Found( option::st::Help ) )
				{
					parser.Usage();
					throw false;
				}

				configFile = new wxFileConfig{ wxEmptyString
					, wxEmptyString
					, findConfigFile( parser )
					, wxEmptyString
					, wxCONFIG_USE_LOCAL_FILE };
			}

			~Options()
			{
				delete configFile;
			}

			bool has( wxString const & option )
			{
				return parser.Found( option );
			}

			template< typename ValueT >
			ValueT getLong( wxString const & option
				, ValueT const & defaultValue = ValueT{} )
			{
				long value;
				ValueT result;

				if ( parser.Found( option, &value ) )
				{
					result = ValueT( value );
				}
				else if ( configFile->HasEntry( option ) )
				{
					configFile->Read( option, &value );
					result = ValueT( value );
				}
				else
				{
					result = defaultValue;
				}

				return result;
			}

			void read( Config & config )
			{
				config.log = getLong( option::st::LogLevel, DefaultLogType );
				config.validate = has( option::st::Validate );

				for ( auto & plugin : list )
				{
					if ( has( makeWxString( plugin.name ) ) )
					{
						config.rendererName = plugin.name;
					}
				}

				config.gpuIndex = getLong( option::st::GpuIndex, DefaultGpuIndex );
			}

			static wxString findConfigFile( wxCmdLineParser const & parser )
			{
				wxString cfg;
				parser.Found( wxT( 'c' ), &cfg );
				return cfg;
			}

		private:
			wxCmdLineParser parser;
			ashes::RendererList list;
			wxFileConfig * configFile{ nullptr };
		};

		static wxString const ShortName = wxT( "OrastusExile" );
		static wxString const LongName = wxT( "Orastus' Exile" );

		OrastusExile::OrastusExile()
			: wxApp{}
		{
		}

		bool OrastusExile::OnInit()
		{
			bool l_return = doParseCommandLine();
			wxDisplay l_display;
			wxRect l_rect = l_display.GetClientArea();
			wxWindow * l_window = nullptr;

			if ( l_return )
			{
				l_return = doInitialiseLocale();
			}

			if ( l_return )
			{
				try
				{
					doLoadImages();
					l_return = doInitialiseCastor();

					if ( l_return )
					{
						l_window = doInitialiseMainFrame();
						l_return = l_window != nullptr;
					}
				}
				catch ( Exception & exc )
				{
					Logger::logError( std::stringstream() << ShortName << " - Initialisation failed : " << exc.getFullDescription() );
					l_return = false;
				}
				catch ( std::exception & exc )
				{
					Logger::logError( std::stringstream() << ShortName << " - Initialisation failed : " << exc.what() );
					l_return = false;
				}
			}

			wxApp::SetTopWindow( l_window );

			if ( !l_return )
			{
				doCleanup();
			}

			return l_return;
		}

		int OrastusExile::OnExit()
		{
			Logger::logInfo( ShortName.ToStdString() + cuT( " - Exit" ) );
			doCleanup();
			return wxApp::OnExit();
		}

		bool OrastusExile::doParseCommandLine()
		{
			wxCmdLineParser l_parser( wxApp::argc, wxApp::argv );
			l_parser.AddSwitch( wxT( "h" ), wxT( "help" ), _( "Displays this help" ) );
			l_parser.AddOption( wxT( "l" ), wxT( "log" ), _( "Defines log level" ), wxCMD_LINE_VAL_NUMBER );
			l_parser.AddParam( _( "The initial scene file" ), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL );
			l_parser.AddSwitch( wxT( "opengl3" ), wxEmptyString, _( "Defines the renderer to OpenGl 3.x." ) );
			l_parser.AddSwitch( wxT( "opengl4" ), wxEmptyString, _( "Defines the renderer to OpenGl 4.x." ) );
			l_parser.AddSwitch( wxT( "vulkan" ), wxEmptyString, _( "Defines the renderer to Vulkan." ) );
			l_parser.AddSwitch( wxT( "direct3d11" ), wxEmptyString, _( "Defines the renderer to Direct3D 11." ) );
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
					l_eLogLevel = DefaultLogType;
				}
				else
				{
					l_eLogLevel = LogType( l_log );
				}

				Logger::initialise( l_eLogLevel );

				if ( l_parser.Found( wxT( "opengl3" ) ) )
				{
					m_rendererType = cuT( "opengl3" );
				}
				else if ( l_parser.Found( wxT( "opengl4" ) ) )
				{
					m_rendererType = cuT( "opengl4" );
				}
				else if ( l_parser.Found( wxT( "vulkan" ) ) )
				{
					m_rendererType = cuT( "vulkan" );
				}
				else if ( l_parser.Found( wxT( "direct3d11" ) ) )
				{
					m_rendererType = cuT( "direct3d11" );
				}
				else if ( l_parser.Found( wxT( "test" ) ) )
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

		bool OrastusExile::doInitialiseLocale()
		{
			long l_lLanguage = wxLANGUAGE_DEFAULT;
			Path l_pathCurrent = File::getExecutableDirectory().getPath();

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

		bool OrastusExile::doInitialiseCastor()
		{
			bool l_return = true;

			if ( !File::directoryExists( Engine::getEngineDirectory() ) )
			{
				File::directoryCreate( Engine::getEngineDirectory() );
			}

			Logger::setFileName( Engine::getEngineDirectory() / ( ShortName.ToStdString() + cuT( ".log" ) ) );
			Logger::logInfo( ShortName.ToStdString() + cuT( " - start" ) );

			m_castor = std::make_unique< Engine >( ShortName.ToStdString()
				, castor3d::Version{ OrastusExile_VERSION_MAJOR, OrastusExile_VERSION_MINOR, OrastusExile_VERSION_BUILD }
				, m_config.validate );
			doLoadPlugins();
			auto & renderers = m_castor->getRenderersList();

			if ( renderers.empty() )
			{
				CU_Exception( "No renderer plug-ins" );
			}
			else if ( std::next( renderers.begin() ) == renderers.end() )
			{
				m_config.rendererName = renderers.begin()->name;
			}

			if ( m_config.rendererName == castor3d::RenderTypeUndefined )
			{
				CU_Exception( "No renderer plug-in selected." );
			}

			if ( auto it = m_castor->getRenderersList().find( m_config.rendererName );
				it != m_castor->getRenderersList().end() )
			{
				m_castor->loadRenderer( castor3d::Renderer{ *m_castor
					, *it
					, {}
					, m_config.gpuIndex } );
			}
			else
			{
				m_castor->loadRenderer( castor3d::Renderer{ *m_castor
					, m_castor->getRenderersList().getSelectedPlugin()
					, {}
					, m_config.gpuIndex } );
			}

			l_return = true;

			if ( l_return )
			{
				l_return = m_castor->loadRenderer( m_rendererType );
			}

			return l_return;
		}

		void OrastusExile::doLoadPlugins()
		{
			static castor::String castor3DLibPrefix{ CU_LibPrefix + castor::String{ cuT( "castor3d" ) } };
			castor::PathArray files;
			castor::File::listDirectoryFiles( castor3d::Engine::getPluginsDirectory(), files );
			castor::PathArray arrayKept;

			// Exclude debug plug-in in release builds, and release plug-ins in debug builds
			for ( auto file : files )
			{
				auto fileName = file.getFileName( true );

				if ( endsWith( fileName, CU_SharedLibExt )
					&& fileName.find( castor3DLibPrefix ) == 0u )
				{
					arrayKept.push_back( file );
				}
			}

			if ( !arrayKept.empty() )
			{
				castor::PathArray arrayFailed;

				for ( auto file : arrayKept )
				{
					if ( !m_castor->getPluginCache().loadPlugin( file ) )
					{
						arrayFailed.push_back( file );
					}
				}

				if ( !arrayFailed.empty() )
				{
					castor::Logger::logWarning( cuT( "Some plug-ins couldn't be loaded :" ) );

					for ( auto file : arrayFailed )
					{
						castor::Logger::logWarning( castor::Path( file ).getFileName() );
					}

					arrayFailed.clear();
				}
			}

			castor::Logger::logInfo( cuT( "Plugins loaded" ) );
		}

		void OrastusExile::doLoadImages()
		{
		}

		wxWindow * OrastusExile::doInitialiseMainFrame()
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

		void OrastusExile::doCleanup()
		{
			doCleanupCastor();
			m_locale.reset();
			ImagesLoader::cleanup();
			Logger::cleanup();
			wxImage::CleanUpHandlers();
		}

		void OrastusExile::doCleanupCastor()
		{
			m_castor.reset();
		}
	}
}