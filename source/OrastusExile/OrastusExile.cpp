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
			bool result = doParseCommandLine();
			wxDisplay display;
			wxRect rect = display.GetClientArea();
			wxWindow * window = nullptr;

			if ( result )
			{
				result = doInitialiseLocale();
			}

			if ( result )
			{
				try
				{
					doLoadImages();
					result = doInitialiseCastor();

					if ( result )
					{
						window = doInitialiseMainFrame();
						result = window != nullptr;
					}
				}
				catch ( Exception & exc )
				{
					Logger::logError( std::stringstream() << ShortName << " - Initialisation failed : " << exc.getFullDescription() );
					result = false;
				}
				catch ( std::exception & exc )
				{
					Logger::logError( std::stringstream() << ShortName << " - Initialisation failed : " << exc.what() );
					result = false;
				}
			}

			wxApp::SetTopWindow( window );

			if ( !result )
			{
				doCleanup();
			}

			return result;
		}

		int OrastusExile::OnExit()
		{
			Logger::logInfo( ShortName.ToStdString() + cuT( " - Exit" ) );
			doCleanup();
			return wxApp::OnExit();
		}

		bool OrastusExile::doParseCommandLine()
		{
			bool result = true;

			try
			{
				main::Options options{ wxApp::argc, wxApp::argv };
				options.read( m_config );
				castor::Logger::initialise( m_config.log );
			}
			catch ( bool )
			{
				result = false;
			}

			return result;
		}

		bool OrastusExile::doInitialiseLocale()
		{
			long language = wxLANGUAGE_DEFAULT;
			Path pathCurrent = File::getExecutableDirectory().getPath();

			// load language if possible, fall back to english otherwise
			if ( wxLocale::IsAvailable( language ) )
			{
				m_locale = std::make_unique< wxLocale >( language, wxLOCALE_LOAD_DEFAULT );
				// add locale search paths
				m_locale->AddCatalogLookupPathPrefix( pathCurrent / cuT( "share" ) / ShortName.ToStdString() );
				m_locale->AddCatalog( ShortName );

				if ( !m_locale->IsOk() )
				{
					std::cerr << "Selected language is wrong" << std::endl;
					language = wxLANGUAGE_ENGLISH;
					m_locale = std::make_unique< wxLocale >( language );
				}
			}
			else
			{
				std::cerr << "The selected language is not supported by your system."
					<< "Try installing support for this language." << std::endl;
				language = wxLANGUAGE_ENGLISH;
				m_locale = std::make_unique< wxLocale >( language );
			}

			return true;
		}

		bool OrastusExile::doInitialiseCastor()
		{
			if ( !File::directoryExists( Engine::getEngineDirectory() ) )
			{
				File::directoryCreate( Engine::getEngineDirectory() );
			}

			Logger::setFileName( Engine::getEngineDirectory() / ( ShortName.ToStdString() + cuT( ".log" ) ) );
			Logger::logInfo( ShortName.ToStdString() + cuT( " - start" ) );
			EngineConfig config{ ShortName.ToStdString()
				, castor3d::Version{ OrastusExile_VERSION_MAJOR, OrastusExile_VERSION_MINOR, OrastusExile_VERSION_BUILD }
				, m_config.validate };

			m_castor = std::make_unique< Engine >( std::move( config ) );
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

			return m_castor->loadRenderer( m_rendererType );
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
