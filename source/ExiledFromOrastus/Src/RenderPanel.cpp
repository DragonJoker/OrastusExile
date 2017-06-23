#include "RenderPanel.hpp"

#include "ExiledFromOrastus.hpp"

#include <Game.hpp>
#include <GridCell.hpp>

#include <Event/Frame/FrameListener.hpp>
#include <Event/UserInput/UserInputListener.hpp>
#include <Material/Material.hpp>
#include <Material/Pass.hpp>
#include <Mesh/Mesh.hpp>
#include <Miscellaneous/Ray.hpp>
#include <Render/RenderWindow.hpp>
#include <Scene/Camera.hpp>
#include <Scene/Scene.hpp>
#include <Scene/SceneNode.hpp>
#include <Texture/TextureUnit.hpp>

using namespace Castor;
using namespace Castor3D;

namespace orastus
{
	namespace main
	{
		namespace
		{
			KeyboardKey ConvertKeyCode( int p_code )
			{
				KeyboardKey l_return = KeyboardKey::eNone;

				if ( p_code < 0x20 )
				{
					switch ( p_code )
					{
					case WXK_BACK:
					case WXK_TAB:
					case WXK_RETURN:
					case WXK_ESCAPE:
						l_return = KeyboardKey( p_code );
						break;
					}
				}
				else if ( p_code == 0x7F )
				{
					l_return = KeyboardKey::eDelete;
				}
				else if ( p_code > 0xFF )
				{
					l_return = KeyboardKey( p_code + int( KeyboardKey::eStart ) - WXK_START );
				}
				else
				{
					// ASCII or extended ASCII character
					l_return = KeyboardKey( p_code );
				}

				return l_return;
			}

			static real const g_camSpeed = 10.0_r;
		}

		RenderPanel::RenderPanel( wxWindow * p_parent, wxSize const & p_size, Game & p_game )
			: wxPanel{ p_parent, wxID_ANY, wxDefaultPosition, p_size }
			, m_game{ p_game }
			, m_timers
			{
				new wxTimer( this, int( TimerID::eUp ) ),
				new wxTimer( this, int( TimerID::eDown ) ),
				new wxTimer( this, int( TimerID::eLeft ) ),
				new wxTimer( this, int( TimerID::eRight ) ),
				new wxTimer( this, int( TimerID::eMouse ) ),
			}
		{
		}

		RenderPanel::~RenderPanel()
		{
			for ( auto & timer : m_timers )
			{
				delete timer;
			}
		}

		void RenderPanel::SetRenderWindow( Castor3D::RenderWindowSPtr p_window )
		{
			m_renderWindow.reset();
			Castor::Size l_sizeWnd{ MakeSize( GetClientSize() ) };

			if ( p_window && p_window->Initialise( l_sizeWnd, MakeWindowHandle( this ) ) )
			{
				Castor::Size l_sizeScreen;
				Castor::System::GetScreenSize( 0, l_sizeScreen );
				GetParent()->SetClientSize( l_sizeWnd.width(), l_sizeWnd.height() );
				l_sizeWnd = MakeSize( GetParent()->GetClientSize() );
				GetParent()->SetPosition( wxPoint( std::abs( int( l_sizeScreen.width() ) - int( l_sizeWnd.width() ) ) / 2, std::abs( int( l_sizeScreen.height() ) - int( l_sizeWnd.height() ) ) / 2 ) );
				SceneSPtr l_scene = p_window->GetScene();

				if ( l_scene )
				{

					m_listener = p_window->GetListener();
					m_renderWindow = p_window;

					if ( p_window )
					{
						auto l_lock = make_unique_lock( l_scene->GetCameraCache() );
						auto l_camera = l_scene->GetCameraCache().begin()->second;
						p_window->GetPickingPass().AddScene( *l_scene, *l_camera );
						m_cameraState = std::make_unique< NodeState >( l_scene->GetListener(), l_camera->GetParent() );
						m_timers[size_t( TimerID::eMouse )]->Start( 30 );
					}
				}
			}
			else if ( m_listener )
			{
				m_listener.reset();
			}
		}

		real RenderPanel::DoTransformX( int x )
		{
			real l_result = real( x );
			RenderWindowSPtr l_window = m_renderWindow.lock();

			if ( l_window )
			{
				l_result *= real( l_window->GetCamera()->GetWidth() ) / GetClientSize().x;
			}

			return l_result;
		}

		real RenderPanel::DoTransformY( int y )
		{
			real l_result = real( y );
			RenderWindowSPtr l_window = m_renderWindow.lock();

			if ( l_window )
			{
				l_result *= real( l_window->GetCamera()->GetHeight() ) / GetClientSize().y;
			}

			return l_result;
		}

		int RenderPanel::DoTransformX( real x )
		{
			int l_result = int( x );
			RenderWindowSPtr l_window = m_renderWindow.lock();

			if ( l_window )
			{
				l_result = int( x * GetClientSize().x / real( l_window->GetCamera()->GetWidth() ) );
			}

			return l_result;
		}

		int RenderPanel::DoTransformY( real y )
		{
			int l_result = int( y );
			RenderWindowSPtr l_window = m_renderWindow.lock();

			if ( l_window )
			{
				l_result = int( y * GetClientSize().y / real( l_window->GetCamera()->GetHeight() ) );
			}

			return l_result;
		}

		void RenderPanel::DoUpdateSelectedGeometry( Castor3D::GeometrySPtr p_geometry )
		{
			auto l_geometry = m_selectedGeometry.lock();

			if ( p_geometry != l_geometry )
			{
				m_selectedGeometry = p_geometry;
				m_selectedTower = m_game.Select( p_geometry );
			}
		}

		void RenderPanel::DoStartTimer( TimerID p_id )
		{
			m_timers[size_t( p_id )]->Start( 10 );
		}

		void RenderPanel::DoStopTimer( TimerID p_id )
		{
			if ( p_id != TimerID::eCount )
			{
				m_timers[size_t( p_id )]->Stop();
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
			EVT_SIZE( RenderPanel::OnSize )
			EVT_MOVE( RenderPanel::OnMove )
			EVT_PAINT( RenderPanel::OnPaint )
			EVT_KEY_DOWN( RenderPanel::OnKeyDown )
			EVT_KEY_UP( RenderPanel::OnKeyUp )
			EVT_LEFT_DOWN( RenderPanel::OnMouseLDown )
			EVT_LEFT_UP( RenderPanel::OnMouseLUp )
			EVT_RIGHT_UP( RenderPanel::OnMouseRUp )
			EVT_MOTION( RenderPanel::OnMouseMove )
			EVT_MOUSEWHEEL( RenderPanel::OnMouseWheel )
			EVT_TIMER( int( TimerID::eUp ), RenderPanel::OnTimerUp )
			EVT_TIMER( int( TimerID::eDown ), RenderPanel::OnTimerDown )
			EVT_TIMER( int( TimerID::eLeft ), RenderPanel::OnTimerLeft )
			EVT_TIMER( int( TimerID::eRight ), RenderPanel::OnTimerRight )
			EVT_TIMER( int( TimerID::eMouse ), RenderPanel::OnMouseTimer )
			END_EVENT_TABLE()

			void RenderPanel::OnSize( wxSizeEvent & p_event )
		{
			RenderWindowSPtr l_window = m_renderWindow.lock();

			if ( l_window )
			{
				l_window->Resize( p_event.GetSize().x, p_event.GetSize().y );
			}
			else
			{
				wxClientDC l_dc( this );
				l_dc.SetBrush( wxBrush( INACTIVE_TAB_COLOUR ) );
				l_dc.SetPen( wxPen( INACTIVE_TAB_COLOUR ) );
				l_dc.DrawRectangle( 0, 0, p_event.GetSize().x, p_event.GetSize().y );
			}

			p_event.Skip();
		}

		void RenderPanel::OnMove( wxMoveEvent & p_event )
		{
			RenderWindowSPtr l_window = m_renderWindow.lock();

			if ( !l_window )
			{
				wxClientDC l_dc( this );
				l_dc.SetBrush( wxBrush( INACTIVE_TAB_COLOUR ) );
				l_dc.SetPen( wxPen( INACTIVE_TAB_COLOUR ) );
				l_dc.DrawRectangle( 0, 0, GetClientSize().x, GetClientSize().y );
			}

			p_event.Skip();
		}

		void RenderPanel::OnPaint( wxPaintEvent & p_event )
		{
			RenderWindowSPtr l_window = m_renderWindow.lock();

			if ( !l_window )
			{
				wxPaintDC l_dc( this );
				l_dc.SetBrush( wxBrush( INACTIVE_TAB_COLOUR ) );
				l_dc.SetPen( wxPen( INACTIVE_TAB_COLOUR ) );
				l_dc.DrawRectangle( 0, 0, GetClientSize().x, GetClientSize().y );
			}

			p_event.Skip();
		}

		void RenderPanel::OnSetFocus( wxFocusEvent & p_event )
		{
			p_event.Skip();
		}

		void RenderPanel::OnKillFocus( wxFocusEvent & p_event )
		{
			DoStopTimer( TimerID::eCount );
			p_event.Skip();
		}

		void RenderPanel::OnKeyDown( wxKeyEvent & p_event )
		{
			auto l_inputListener = wxGetApp().GetCastor().GetUserInputListener();

			if ( !l_inputListener || !l_inputListener->FireKeyDown( ConvertKeyCode( p_event.GetKeyCode() )
				, p_event.ControlDown()
				, p_event.AltDown()
				, p_event.ShiftDown() ) )
			{
				switch ( p_event.GetKeyCode() )
				{
				case WXK_LEFT:
				case 'Q':
					DoStartTimer( TimerID::eLeft );
					break;

				case WXK_RIGHT:
				case 'D':
					DoStartTimer( TimerID::eRight );
					break;

				case WXK_UP:
				case 'Z':
					DoStartTimer( TimerID::eUp );
					break;

				case WXK_DOWN:
				case 'S':
					DoStartTimer( TimerID::eDown );
					break;
				}
			}

			p_event.Skip();
		}

		void RenderPanel::OnKeyUp( wxKeyEvent & p_event )
		{
			auto l_inputListener = wxGetApp().GetCastor().GetUserInputListener();

			if ( !l_inputListener || !l_inputListener->FireKeyUp( ConvertKeyCode( p_event.GetKeyCode() )
				, p_event.ControlDown()
				, p_event.AltDown()
				, p_event.ShiftDown() ) )
			{
				switch ( p_event.GetKeyCode() )
				{
				case WXK_F1:
					m_listener->PostEvent( MakeFunctorEvent( EventType::ePostRender
						, [this]()
						{
							if ( m_game.IsRunning() )
							{
								m_game.Help();
							}
						} ) );
					break;

				case WXK_RETURN:
				case WXK_NUMPAD_ENTER:
					m_listener->PostEvent( MakeFunctorEvent( EventType::ePostRender
						, [this]()
						{
							if ( m_game.IsEnded() )
							{
								m_game.Reset();
								m_game.Start();
							}
							else if ( !m_game.IsStarted() )
							{
								m_game.Start();
							}
						} ) );
					break;

				case WXK_SPACE:
					m_listener->PostEvent( MakeFunctorEvent( EventType::ePostRender
						, [this]()
						{
							if ( m_game.IsStarted() )
							{
								if ( m_game.IsPaused() )
								{
									m_game.Resume();
								}
								else
								{
									m_game.Pause();
								}
							}
						} ) );
					break;

				case WXK_LEFT:
				case 'Q':
					DoStopTimer( TimerID::eLeft );
					break;

				case WXK_RIGHT:
				case 'D':
					DoStopTimer( TimerID::eRight );
					break;

				case WXK_UP:
				case 'Z':
					DoStopTimer( TimerID::eUp );
					break;

				case WXK_DOWN:
				case 'S':
					DoStopTimer( TimerID::eDown );
					break;
				}
			}

			p_event.Skip();
		}

		void RenderPanel::OnMouseLDown( wxMouseEvent & p_event )
		{
			m_mouseLeftDown = true;
			auto l_window = GetRenderWindow();

			if ( l_window )
			{
				if ( m_game.IsRunning() )
				{
					auto l_inputListener = wxGetApp().GetCastor().GetUserInputListener();

					if ( !l_inputListener || !l_inputListener->FireMouseButtonPushed( MouseButton::eLeft ) )
					{
						m_x = DoTransformX( p_event.GetX() );
						m_y = DoTransformY( p_event.GetY() );
						m_oldX = m_x;
						m_oldY = m_y;
					}
				}
			}
		}

		void RenderPanel::OnMouseLUp( wxMouseEvent & p_event )
		{
			m_mouseLeftDown = false;
			auto l_window = GetRenderWindow();

			if ( l_window )
			{
				if ( m_game.IsRunning() )
				{
					auto l_inputListener = wxGetApp().GetCastor().GetUserInputListener();

					if ( !l_inputListener || !l_inputListener->FireMouseButtonReleased( MouseButton::eLeft ) )
					{
						m_x = DoTransformX( p_event.GetX() );
						m_y = DoTransformY( p_event.GetY() );
						m_oldX = m_x;
						m_oldY = m_y;
						m_listener->PostEvent( MakeFunctorEvent( EventType::ePreRender
							, [this, l_window]()
							{
								Camera & l_camera = *l_window->GetCamera();
								l_camera.Update();
								auto l_type = l_window->GetPickingPass().Pick( Position{ int( m_x ), int( m_y ) }, l_camera );

								if ( l_type != PickingPass::NodeType::eNone
									&& l_type != PickingPass::NodeType::eBillboard )
								{
									DoUpdateSelectedGeometry( l_window->GetPickingPass().GetPickedGeometry() );
								}
								else
								{
									m_selectedGeometry.reset();
									m_game.Unselect();
								}
							} ) );
					}
				}
			}

			p_event.Skip();
		}

		void RenderPanel::OnMouseRDown( wxMouseEvent & p_event )
		{
			auto l_window = GetRenderWindow();

			if ( l_window )
			{
				auto l_inputListener = wxGetApp().GetCastor().GetUserInputListener();

				if ( l_inputListener )
				{
					l_inputListener->FireMouseButtonPushed( MouseButton::eRight );
				}
			}

			p_event.Skip();
		}

		void RenderPanel::OnMouseRUp( wxMouseEvent & p_event )
		{
			auto l_window = GetRenderWindow();

			if ( l_window )
			{
				auto l_inputListener = wxGetApp().GetCastor().GetUserInputListener();

				if ( l_inputListener )
				{
					l_inputListener->FireMouseButtonReleased( MouseButton::eRight );
				}
			}

			p_event.Skip();
		}

		void RenderPanel::OnMouseMove( wxMouseEvent & p_event )
		{
			m_x = DoTransformX( p_event.GetX() );
			m_y = DoTransformY( p_event.GetY() );
			auto l_window = GetRenderWindow();

			if ( l_window )
			{
				if ( m_game.IsRunning() )
				{
					auto l_inputListener = wxGetApp().GetCastor().GetUserInputListener();

					if ( !l_inputListener || !l_inputListener->FireMouseMove( Position{ int( m_x ), int( m_y ) } ) )
					{
						static real constexpr l_mult = 4.0_r;
						real l_deltaX = 0.0_r;
						real l_deltaY = std::min( 1.0_r / l_mult, 1.0_r ) * ( m_oldY - m_y ) / l_mult;

						if ( m_mouseLeftDown )
						{
							m_cameraState->AddAngularVelocity( Point2r{ -l_deltaY, l_deltaX } );
						}
					}
				}
			}

			m_oldX = m_x;
			m_oldY = m_y;
			p_event.Skip();
		}

		void RenderPanel::OnMouseWheel( wxMouseEvent & p_event )
		{
			int l_wheelRotation = p_event.GetWheelRotation();

			auto l_inputListener = wxGetApp().GetCastor().GetUserInputListener();

			if ( !l_inputListener || !l_inputListener->FireMouseWheel( Position( 0, l_wheelRotation ) ) )
			{
				if ( l_wheelRotation < 0 )
				{
					m_cameraState->AddScalarVelocity( Point3r{ 0.0_r, 0.0_r, -g_camSpeed } );
				}
				else
				{
					m_cameraState->AddScalarVelocity( Point3r{ 0.0_r, 0.0_r, g_camSpeed } );
				}
			}

			p_event.Skip();
		}

		void RenderPanel::OnMouseTimer( wxTimerEvent & p_event )
		{
			if ( m_game.IsRunning() && m_cameraState )
			{
				m_cameraState->Update();
			}

			p_event.Skip();
		}

		void RenderPanel::OnTimerUp( wxTimerEvent & p_event )
		{
			m_cameraState->AddScalarVelocity( Point3r{ 0.0_r, g_camSpeed, 0.0_r } );
			p_event.Skip();
		}

		void RenderPanel::OnTimerDown( wxTimerEvent & p_event )
		{
			m_cameraState->AddScalarVelocity( Point3r{ 0.0_r, -g_camSpeed, 0.0_r } );
			p_event.Skip();
		}

		void RenderPanel::OnTimerLeft( wxTimerEvent & p_event )
		{
			m_cameraState->AddScalarVelocity( Point3r{ g_camSpeed, 0.0_r, 0.0_r } );
			p_event.Skip();
		}

		void RenderPanel::OnTimerRight( wxTimerEvent & p_event )
		{
			m_cameraState->AddScalarVelocity( Point3r{ -g_camSpeed, 0.0_r, 0.0_r } );
			p_event.Skip();
		}
	}
}
