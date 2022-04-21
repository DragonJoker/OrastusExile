#include "RenderPanel.hpp"

#include "OrastusExile.hpp"

#include <GameEngine/Game.hpp>
#include <GameEngine/GridCell.hpp>

#include <Castor3D/Event/Frame/FrameListener.hpp>
#include <Castor3D/Event/UserInput/UserInputListener.hpp>
#include <Castor3D/Material/Material.hpp>
#include <Castor3D/Material/Pass/Pass.hpp>
#include <Castor3D/Material/Texture/TextureUnit.hpp>
#include <Castor3D/Model/Mesh/Mesh.hpp>
#include <Castor3D/Render/RenderTarget.hpp>
#include <Castor3D/Render/RenderWindow.hpp>
#include <Castor3D/Scene/Camera.hpp>
#include <Castor3D/Scene/Scene.hpp>
#include <Castor3D/Scene/SceneNode.hpp>

using namespace castor;
using namespace castor3d;

namespace orastus
{
	namespace main
	{
		namespace
		{
			KeyboardKey convertKeyCode( int code )
			{
				KeyboardKey result = KeyboardKey::eNone;

				if ( code < 0x20 )
				{
					switch ( code )
					{
					case WXK_BACK:
					case WXK_TAB:
					case WXK_RETURN:
					case WXK_ESCAPE:
						result = KeyboardKey( code );
						break;
					}
				}
				else if ( code == 0x7F )
				{
					result = KeyboardKey::eDelete;
				}
				else if ( code > 0xFF )
				{
					result = KeyboardKey( code + int( KeyboardKey::eStart ) - WXK_START );
				}
				else
				{
					// ASCII or extended ASCII character
					result = KeyboardKey( code );
				}

				return result;
			}
		}

		RenderPanel::RenderPanel( wxWindow * parent, wxSize const & size, Game & game )
			: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
			, m_timers
			{
				new wxTimer( this, int( TimerID::eUp ) ),
				new wxTimer( this, int( TimerID::eDown ) ),
				new wxTimer( this, int( TimerID::eLeft ) ),
				new wxTimer( this, int( TimerID::eRight ) ),
				new wxTimer( this, int( TimerID::eMouse ) ),
			}
			, m_game{ game }
		{
			m_renderWindow = castor::makeUnique< castor3d::RenderWindow >( cuT( "RenderPanel" )
				, wxGetApp().getCastor()
				, castor::Size{ uint32_t( GetClientSize().x ), uint32_t( GetClientSize().y ) }
				, makeWindowHandle( this ) );
		}

		RenderPanel::~RenderPanel()
		{
			for ( auto & timer : m_timers )
			{
				delete timer;
			}
		}

		void RenderPanel::reset()
		{
			m_renderWindow->cleanup();
		}

		void RenderPanel::setRenderTarget( castor3d::RenderTargetSPtr target )
		{
			m_listener = m_renderWindow->getListener();
			m_renderWindow->initialise( target );
			auto scene = target->getScene();

			if ( scene )
			{
				castor::Size sizeWnd{ makeSize( GetClientSize() ) };
				castor::Size sizeScreen;
				castor::System::getScreenSize( 0, sizeScreen );
				GetParent()->SetClientSize( int( sizeWnd.getWidth() )
					, int( sizeWnd.getHeight() ) );
				sizeWnd = makeSize( GetParent()->GetClientSize() );
				GetParent()->SetPosition( wxPoint( std::abs( int( sizeScreen.getWidth() ) - int( sizeWnd.getWidth() ) ) / 2
					, std::abs( int( sizeScreen.getHeight() ) - int( sizeWnd.getHeight() ) ) / 2 ) );
				auto camera = target->getCamera();

				if ( camera )
				{
					m_renderWindow->addPickingScene( *scene );
					m_camera = camera;
					m_cameraState = std::make_unique< NodeState >( scene->getListener(), camera->getParent() );
					m_timers[size_t( TimerID::eMouse )]->Start( 30 );
				}

				m_scene = scene;
			}
		}

		float RenderPanel::doTransformX( int x )
		{
			float result = float( x );

			if ( auto camera = m_camera.lock() )
			{
				result *= float( camera->getWidth() ) / float( GetClientSize().x );
			}

			return result;
		}

		float RenderPanel::doTransformY( int y )
		{
			float result = float( y );

			if ( auto camera = m_camera.lock() )
			{
				result *= float( camera->getHeight() ) / float( GetClientSize().y );
			}

			return result;
		}

		int RenderPanel::doTransformX( float x )
		{
			int result = int( x );

			if ( auto camera = m_camera.lock() )
			{
				result = int( x * float( GetClientSize().x ) / float( camera->getWidth() ) );
			}

			return result;
		}

		int RenderPanel::doTransformY( float y )
		{
			int result = int( y );

			if ( auto camera = m_camera.lock() )
			{
				result = int( y * float( GetClientSize().y ) / float( camera->getHeight() ) );
			}

			return result;
		}

		void RenderPanel::doStartTimer( TimerID id )
		{
			m_timers[size_t( id )]->Start( 10 );
		}

		void RenderPanel::doStopTimer( TimerID id )
		{
			if ( id != TimerID::eCount )
			{
				m_timers[size_t( id )]->Stop();
			}
			else
			{
				for ( size_t i = 0; i < size_t( TimerID::eMouse ); i++ )
				{
					m_timers[i]->Stop();
				}
			}
		}

		BEGIN_EVENT_TABLE( RenderPanel, wxPanel )
			EVT_SIZE( RenderPanel::onSize )
			EVT_MOVE( RenderPanel::onMove )
			EVT_PAINT( RenderPanel::onPaint )
			EVT_KEY_DOWN( RenderPanel::onKeyDown )
			EVT_KEY_UP( RenderPanel::onKeyUp )
			EVT_LEFT_DOWN( RenderPanel::onMouseLDown )
			EVT_LEFT_UP( RenderPanel::onMouseLUp )
			EVT_RIGHT_UP( RenderPanel::onMouseRUp )
			EVT_MOTION( RenderPanel::onMouseMove )
			EVT_MOUSEWHEEL( RenderPanel::onMouseWheel )
			EVT_TIMER( int( TimerID::eUp ), RenderPanel::onTimerUp )
			EVT_TIMER( int( TimerID::eDown ), RenderPanel::onTimerDown )
			EVT_TIMER( int( TimerID::eLeft ), RenderPanel::onTimerLeft )
			EVT_TIMER( int( TimerID::eRight ), RenderPanel::onTimerRight )
			EVT_TIMER( int( TimerID::eMouse ), RenderPanel::onMouseTimer )
		END_EVENT_TABLE()

		void RenderPanel::onSize( wxSizeEvent & event )
		{
			if ( m_resizeWindow && m_renderWindow )
			{
				m_renderWindow->resize( uint32_t( event.GetSize().x )
					, uint32_t( event.GetSize().y ) );
			}
			else
			{
				wxClientDC dc( this );
				dc.SetBrush( wxBrush( INACTIVE_TAB_COLOUR ) );
				dc.SetPen( wxPen( INACTIVE_TAB_COLOUR ) );
				dc.DrawRectangle( 0, 0, event.GetSize().x, event.GetSize().y );
			}

			event.Skip();
		}

		void RenderPanel::onMove( wxMoveEvent & event )
		{
			event.Skip();
		}

		void RenderPanel::onPaint( wxPaintEvent & event )
		{
			event.Skip();
		}

		void RenderPanel::onSetFocus( wxFocusEvent & event )
		{
			event.Skip();
		}

		void RenderPanel::onKillFocus( wxFocusEvent & event )
		{
			doStopTimer( TimerID::eCount );
			event.Skip();
		}

		void RenderPanel::onKeyDown( wxKeyEvent & event )
		{
			auto inputListener = wxGetApp().getCastor().getUserInputListener();

			if ( !inputListener || !inputListener->fireKeydown( convertKeyCode( event.GetKeyCode() )
				, event.ControlDown()
				, event.AltDown()
				, event.ShiftDown() ) )
			{
				switch ( event.GetKeyCode() )
				{
				case WXK_LEFT:
				case 'Q':
					doStartTimer( TimerID::eLeft );
					break;
				case WXK_RIGHT:
				case 'D':
					doStartTimer( TimerID::eRight );
					break;
				case WXK_UP:
				case 'Z':
					doStartTimer( TimerID::eUp );
					break;
				case WXK_DOWN:
				case 'S':
					doStartTimer( TimerID::eDown );
					break;
				}
			}

			event.Skip();
		}

		void RenderPanel::onKeyUp( wxKeyEvent & event )
		{
			auto inputListener = wxGetApp().getCastor().getUserInputListener();

			if ( !inputListener || !inputListener->fireKeyUp( convertKeyCode( event.GetKeyCode() )
				, event.ControlDown()
				, event.AltDown()
				, event.ShiftDown() ) )
			{
				switch ( event.GetKeyCode() )
				{
				case WXK_F1:
					m_listener->postEvent( makeCpuFunctorEvent( EventType::ePostRender
						, [this]()
						{
							if ( m_game.isRunning() )
							{
								m_game.help();
							}
						} ) );
					break;
				case WXK_RETURN:
				case WXK_NUMPAD_ENTER:
					m_listener->postEvent( makeCpuFunctorEvent( EventType::ePostRender
						, [this]()
						{
							if ( m_game.isEnded() )
							{
								m_game.reset();
								m_game.start();
							}
							else if ( !m_game.isStarted() )
							{
								m_game.start();
							}
						} ) );
					break;
				case WXK_SPACE:
					m_listener->postEvent( makeCpuFunctorEvent( EventType::ePostRender
						, [this]()
						{
							if ( m_game.isStarted() )
							{
								if ( m_game.isPaused() )
								{
									m_game.resume();
								}
								else
								{
									m_game.pause();
								}
							}
						} ) );
					break;
				case WXK_LEFT:
				case 'Q':
					doStopTimer( TimerID::eLeft );
					break;
				case WXK_RIGHT:
				case 'D':
					doStopTimer( TimerID::eRight );
					break;
				case WXK_UP:
				case 'Z':
					doStopTimer( TimerID::eUp );
					break;
				case WXK_DOWN:
				case 'S':
					doStopTimer( TimerID::eDown );
					break;
				case '1':
					m_listener->postEvent( makeCpuFunctorEvent( EventType::ePreRender
						, [this]()
						{
							m_game.createDirectTower();
						} ) );
					break;
				case '2':
					m_listener->postEvent( makeCpuFunctorEvent( EventType::ePreRender
						, [this]()
						{
							m_game.createSplashTower();
						} ) );
					break;
				}
			}

			event.Skip();
		}

		void RenderPanel::onMouseLDown( wxMouseEvent & event )
		{
			m_mouseLeftDown = true;
			m_x = doTransformX( event.GetX() );
			m_y = doTransformY( event.GetY() );
			m_oldX = m_x;
			m_oldY = m_y;

			if ( m_game.isRunning() )
			{
				if ( auto inputListener = wxGetApp().getCastor().getUserInputListener() )
				{
					inputListener->fireMouseButtonPushed( MouseButton::eLeft );
				}
			}
		}

		void RenderPanel::onMouseLUp( wxMouseEvent & event )
		{
			m_mouseLeftDown = false;
			m_x = doTransformX( event.GetX() );
			m_y = doTransformY( event.GetY() );
			m_oldX = m_x;
			m_oldY = m_y;

			if ( m_game.isRunning() )
			{
				auto inputListener = wxGetApp().getCastor().getUserInputListener();

				if ( !inputListener || !inputListener->fireMouseButtonReleased( MouseButton::eLeft ) )
				{
					auto x = m_oldX;
					auto y = m_oldY;
					m_listener->postEvent( makeCpuFunctorEvent( EventType::ePreRender
						, [this, x, y]()
						{
							Camera & camera = *m_renderWindow->getCamera();
							camera.update();
							auto type = m_renderWindow->pick( Position{ int( x ), int( y ) } );

							if ( type != PickNodeType::eNone
								&& type != PickNodeType::eBillboard )
							{
								auto picked = m_renderWindow->getPickedGeometry();
								auto geometry = m_selectedGeometry.lock();

								if ( picked != geometry )
								{
									m_selectedGeometry = picked;
									m_selectedTower = m_game.select( picked );
								}
							}
							else
							{
								m_selectedGeometry.reset();
								m_game.unselect();
							}
						} ) );
				}
			}

			event.Skip();
		}

		void RenderPanel::onMouseRDown( wxMouseEvent & event )
		{
			if ( auto inputListener = wxGetApp().getCastor().getUserInputListener() )
			{
				inputListener->fireMouseButtonPushed( MouseButton::eRight );
			}

			event.Skip();
		}

		void RenderPanel::onMouseRUp( wxMouseEvent & event )
		{
			if ( auto inputListener = wxGetApp().getCastor().getUserInputListener() )
			{
				inputListener->fireMouseButtonReleased( MouseButton::eRight );
			}

			event.Skip();
		}

		void RenderPanel::onMouseMove( wxMouseEvent & event )
		{
			m_x = doTransformX( event.GetX() );
			m_y = doTransformY( event.GetY() );

			if ( m_game.isRunning() )
			{
				auto inputListener = wxGetApp().getCastor().getUserInputListener();

				if ( !inputListener || !inputListener->fireMouseMove( Position{ int( m_x ), int( m_y ) } ) )
				{
					static float constexpr mult = 4.0f;
					float deltaX = 0.0f;
					float deltaY = std::min( 1.0f / mult, 1.0f ) * ( m_oldY - m_y ) / mult;

					if ( m_mouseLeftDown )
					{
						m_cameraState->addAngularVelocity( Point2f{ -deltaY, deltaX } );
					}
				}
			}

			m_oldX = m_x;
			m_oldY = m_y;
			event.Skip();
		}

		void RenderPanel::onMouseWheel( wxMouseEvent & event )
		{
			int wheelRotation = event.GetWheelRotation();
			auto inputListener = wxGetApp().getCastor().getUserInputListener();

			if ( !inputListener || !inputListener->fireMouseWheel( Position( 0, wheelRotation ) ) )
			{
				if ( wheelRotation < 0 )
				{
					m_cameraState->addScalarVelocity( Point3f{ 0.0f, 0.0f, -m_camSpeed } );
				}
				else
				{
					m_cameraState->addScalarVelocity( Point3f{ 0.0f, 0.0f, m_camSpeed } );
				}
			}

			event.Skip();
		}

		void RenderPanel::onMouseTimer( wxTimerEvent & event )
		{
			if ( m_game.isRunning() && m_cameraState )
			{
				m_cameraState->update();
			}

			event.Skip();
		}

		void RenderPanel::onTimerUp( wxTimerEvent & event )
		{
			m_cameraState->addScalarVelocity( Point3f{ 0.0f, m_camSpeed, 0.0f } );
			event.Skip();
		}

		void RenderPanel::onTimerDown( wxTimerEvent & event )
		{
			m_cameraState->addScalarVelocity( Point3f{ 0.0f, -m_camSpeed, 0.0f } );
			event.Skip();
		}

		void RenderPanel::onTimerLeft( wxTimerEvent & event )
		{
			m_cameraState->addScalarVelocity( Point3f{ m_camSpeed, 0.0f, 0.0f } );
			event.Skip();
		}

		void RenderPanel::onTimerRight( wxTimerEvent & event )
		{
			m_cameraState->addScalarVelocity( Point3f{ -m_camSpeed, 0.0f, 0.0f } );
			event.Skip();
		}
	}
}
