#include "MainFrame.hpp"

#include "RenderPanel.hpp"
#include "OrastusExile.hpp"

#include <GameEngine/Game.hpp>

#include <Castor3D/Event/Frame/CpuFunctorEvent.hpp>
#include <Castor3D/Render/RenderLoop.hpp>
#include <Castor3D/Render/RenderTarget.hpp>
#include <Castor3D/Render/RenderWindow.hpp>
#include <Castor3D/Scene/SceneFileParser.hpp>

#include <wx/sizer.h>

using namespace castor;
using namespace castor3d;

namespace orastus
{
	namespace main
	{
		namespace
		{
			static const wxSize MainFrameSize{ 1024, 768 };
			static const bool CASTOR3D_THREADED = true;

			typedef enum eID
			{
				eID_RENDER_TIMER,
			}	eID;

			void doUpdate( Game & game )
			{
				if ( !wxGetApp().getCastor().isCleaned() )
				{
					game.update();
					wxGetApp().getCastor().postEvent( makeCpuFunctorEvent( EventType::ePostRender, [&game]()
					{
						doUpdate( game );
					} ) );
				}
			}
		}

		MainFrame::MainFrame()
			: wxFrame{ nullptr
				, wxID_ANY
				, ApplicationName
				, wxDefaultPosition
				, MainFrameSize
				, wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxRESIZE_BORDER | wxMAXIMIZE_BOX }
		{
			SetClientSize( MainFrameSize );
			Show( true );

			try
			{
				wxGetApp().getCastor().initialise( 120, CASTOR3D_THREADED );
				doLoadScene();
				wxBoxSizer * sizer{ new wxBoxSizer{ wxHORIZONTAL } };
				sizer->Add( m_panel.get(), wxSizerFlags( 1 ).Shaped().Centre() );
				sizer->SetSizeHints( this );
				SetSizer( sizer );
			}
			catch ( std::exception & exc )
			{
				wxMessageBox( exc.what() );
			}
		}

		MainFrame::~MainFrame()
		{
		}

		void MainFrame::doLoadScene()
		{
			auto & engine = wxGetApp().getCastor();
			auto target = doLoadScene( engine
				, File::getExecutableDirectory().getPath() / cuT( "share" ) / cuT( "GameEngine" ) / cuT( "GameEngine.zip" )
				, engine.getRenderLoop().getWantedFps()
				, engine.isThreaded() );

			if ( target )
			{
				m_game = std::make_unique< Game >( *target->getScene() );
				m_panel = wxMakeWindowPtr< RenderPanel >( this, MainFrameSize, *m_game );
				m_panel->setRenderTarget( target );
				auto & window = m_panel->getRenderWindow();
				wxSize size = makeWxSize( window.getSize() );

				if ( window.isFullscreen() )
				{
					ShowFullScreen( true, wxFULLSCREEN_ALL );
				}

				if ( !IsMaximized() )
				{
					SetClientSize( size );
				}
				else
				{
					Maximize( false );
					SetClientSize( size );
					Maximize();
				}

				Logger::logInfo( cuT( "Scene file read" ) );

#if wxCHECK_VERSION( 2, 9, 0 )

				size = GetClientSize();
				SetMinClientSize( size );

#endif

				if ( engine.isThreaded() )
				{
					engine.getRenderLoop().beginRendering();
					engine.postEvent( makeCpuFunctorEvent( EventType::ePostRender
						, [this]()
						{
							doUpdate( *m_game );
						} ) );
				}
				else
				{
					m_timer = new wxTimer( this, eID_RENDER_TIMER );
					m_timer->Start( 1000 / int( engine.getRenderLoop().getWantedFps() ), true );
				}
			}
		}

		RenderTargetSPtr MainFrame::doLoadScene( Engine & engine
			, castor::Path const & fileName
			, uint32_t wantedFps
			, bool threaded )
		{
			RenderTargetSPtr result;

			if ( File::fileExists( fileName ) )
			{
				Logger::logInfo( cuT( "Loading scene file : " ) + fileName );

				if ( fileName.getExtension() == cuT( "cscn" ) || fileName.getExtension() == cuT( "zip" ) )
				{
					try
					{
						castor3d::SceneFileParser parser( engine );
						auto preprocessed = parser.processFile( "Castor3D", fileName );

						if ( preprocessed.parse() )
						{
							result = parser.getRenderWindow().renderTarget;
						}
						else
						{
							castor::Logger::logWarning( cuT( "Can't read scene file" ) );
						}
					}
					catch ( std::exception & exc )
					{
						wxMessageBox( _( "Failed to parse the scene file, with following error:" ) + wxString( wxT( "\n" ) ) + wxString( exc.what(), wxMBConvLibc() ) );
					}
				}
			}
			else
			{
				wxMessageBox( _( "Scene file doesn't exist :" ) + wxString( wxT( "\n" ) ) + fileName );
			}

			return result;
		}

		BEGIN_EVENT_TABLE( MainFrame, wxFrame )
			EVT_PAINT( MainFrame::onPaint )
			EVT_CLOSE( MainFrame::onClose )
			EVT_ERASE_BACKGROUND( MainFrame::onEraseBackground )
			EVT_TIMER( eID_RENDER_TIMER, MainFrame::onRenderTimer )
			EVT_KEY_DOWN( MainFrame::onKeyDown )
			EVT_KEY_UP( MainFrame::onKeyUp )
			EVT_LEFT_DOWN( MainFrame::OnMouseLdown )
			EVT_LEFT_UP( MainFrame::OnMouseLUp )
			EVT_RIGHT_UP( MainFrame::OnMouseRUp )
		END_EVENT_TABLE()

		void MainFrame::onPaint( wxPaintEvent & event )
		{
			wxPaintDC paintDC( this );
			event.Skip();
		}

		void MainFrame::onClose( wxCloseEvent & event )
		{
			auto & engine = wxGetApp().getCastor();
			Hide();

			if ( m_timer )
			{
				m_timer->Stop();
				delete m_timer;
				m_timer = nullptr;
			}

			if ( m_panel )
			{
				if ( engine.isThreaded() )
				{
					engine.getRenderLoop().pause();
				}

				m_panel->reset();

				if ( engine.isThreaded() )
				{
					engine.getRenderLoop().resume();
				}
			}

			engine.cleanup();

			if ( m_panel )
			{
				m_panel->Close( true );
				m_panel = nullptr;
			}

			DestroyChildren();
			event.Skip();
		}

		void MainFrame::onEraseBackground( wxEraseEvent & event )
		{
			event.Skip();
		}

		void MainFrame::onRenderTimer( wxTimerEvent & event )
		{
			auto & engine = wxGetApp().getCastor();

			if ( !engine.isCleaned() )
			{
				if ( !engine.isThreaded() )
				{
					engine.getRenderLoop().renderSyncFrame();
					m_game->update();
					m_timer->Start( 1000 / int( engine.getRenderLoop().getWantedFps() ), true );
				}
			}
		}

		void MainFrame::onKeyDown( wxKeyEvent & event )
		{
			if ( m_panel )
			{
				m_panel->GetEventHandler()->QueueEvent( new wxKeyEvent{ event } );
				event.Skip();
			}
		}

		void MainFrame::onKeyUp( wxKeyEvent & event )
		{
			if ( m_panel )
			{
				m_panel->GetEventHandler()->QueueEvent( new wxKeyEvent{ event } );
				event.Skip();
			}
		}

		void MainFrame::OnMouseLdown( wxMouseEvent & event )
		{
			if ( m_panel )
			{
				m_panel->GetEventHandler()->QueueEvent( new wxMouseEvent{ event } );
				event.Skip();
			}
		}

		void MainFrame::OnMouseLUp( wxMouseEvent & event )
		{
			if ( m_panel )
			{
				m_panel->GetEventHandler()->QueueEvent( new wxMouseEvent{ event } );
				event.Skip();
			}
		}

		void MainFrame::OnMouseRUp( wxMouseEvent & event )
		{
			if ( m_panel )
			{
				m_panel->GetEventHandler()->QueueEvent( new wxMouseEvent{ event } );
				event.Skip();
			}
		}

		void MainFrame::OnMouseWheel( wxMouseEvent & event )
		{
			if ( m_panel )
			{
				m_panel->GetEventHandler()->QueueEvent( new wxMouseEvent{ event } );
				event.Skip();
			}
		}
	}
}
