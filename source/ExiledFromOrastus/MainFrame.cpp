#include "MainFrame.hpp"

#include "RenderPanel.hpp"
#include "ExiledFromOrastus.hpp"

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

			void doUpdate( Game & p_game )
			{
				if ( !wxGetApp().getCastor().isCleaned() )
				{
					p_game.update();
					wxGetApp().getCastor().postEvent( makeCpuFunctorEvent( EventType::ePostRender, [&p_game]()
					{
						doUpdate( p_game );
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
				wxBoxSizer * l_sizer{ new wxBoxSizer{ wxHORIZONTAL } };
				l_sizer->Add( m_panel.get(), wxSizerFlags( 1 ).Shaped().Centre() );
				l_sizer->SetSizeHints( this );
				SetSizer( l_sizer );
			}
			catch ( std::exception & p_exc )
			{
				wxMessageBox( p_exc.what() );
			}
		}

		MainFrame::~MainFrame()
		{
		}

		void MainFrame::doLoadScene()
		{
			auto & l_engine = wxGetApp().getCastor();
			auto target = doLoadScene( l_engine
				, File::getExecutableDirectory().getPath() / cuT( "share" ) / cuT( "GameEngine" ) / cuT( "GameEngine.zip" )
				, l_engine.getRenderLoop().getWantedFps()
				, l_engine.isThreaded() );

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

				if ( l_engine.isThreaded() )
				{
					l_engine.getRenderLoop().beginRendering();
					l_engine.postEvent( makeCpuFunctorEvent( EventType::ePostRender, [this]()
					{
						doUpdate( *m_game );
					} ) );
				}
				else
				{
					m_timer = new wxTimer( this, eID_RENDER_TIMER );
					m_timer->Start( 1000 / int( l_engine.getRenderLoop().getWantedFps() ), true );
				}
			}
		}

		RenderTargetSPtr MainFrame::doLoadScene( Engine & p_engine
			, castor::Path const & p_fileName
			, uint32_t p_wantedFps
			, bool p_threaded )
		{
			RenderTargetSPtr l_return;

			if ( File::fileExists( p_fileName ) )
			{
				Logger::logInfo( cuT( "Loading scene file : " ) + p_fileName );

				if ( p_fileName.getExtension() == cuT( "cscn" ) || p_fileName.getExtension() == cuT( "zip" ) )
				{
					try
					{
						castor3d::SceneFileParser parser( p_engine );
						auto preprocessed = parser.processFile( "Castor3D", p_fileName );

						if ( preprocessed.parse() )
						{
							l_return = parser.getRenderWindow().renderTarget;
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
				wxMessageBox( _( "Scene file doesn't exist :" ) + wxString( wxT( "\n" ) ) + p_fileName );
			}

			return l_return;
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

		void MainFrame::onPaint( wxPaintEvent & p_event )
		{
			wxPaintDC l_paintDC( this );
			p_event.Skip();
		}

		void MainFrame::onClose( wxCloseEvent & p_event )
		{
			auto & l_engine = wxGetApp().getCastor();
			Hide();

			if ( m_timer )
			{
				m_timer->Stop();
				delete m_timer;
				m_timer = nullptr;
			}

			if ( m_panel )
			{
				if ( l_engine.isThreaded() )
				{
					l_engine.getRenderLoop().pause();
				}

				m_panel->reset();

				if ( l_engine.isThreaded() )
				{
					l_engine.getRenderLoop().resume();
				}
			}

			l_engine.cleanup();

			if ( m_panel )
			{
				m_panel->Close( true );
				m_panel = nullptr;
			}

			DestroyChildren();
			p_event.Skip();
		}

		void MainFrame::onEraseBackground( wxEraseEvent & p_event )
		{
			p_event.Skip();
		}

		void MainFrame::onRenderTimer( wxTimerEvent & p_event )
		{
			auto & l_castor = wxGetApp().getCastor();

			if ( !l_castor.isCleaned() )
			{
				if ( !l_castor.isThreaded() )
				{
					l_castor.getRenderLoop().renderSyncFrame();
					m_game->update();
					m_timer->Start( 1000 / int( l_castor.getRenderLoop().getWantedFps() ), true );
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
