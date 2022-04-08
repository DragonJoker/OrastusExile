/**
See licence file in root folder, MIT.txt
*/
#pragma once

#include "OrastusExilePrerequisites.hpp"
#include "NodeState.hpp"

#include <GameEngine/ECS/Entity.hpp>

#include <Castor3D/Engine.hpp>

#include <wx/panel.h>
#include <wx/timer.h>

namespace orastus
{
	namespace main
	{
		enum class TimerID
		{
			eUp,
			eDown,
			eLeft,
			eRight,
			eMouse,
			eCount,
		};

		class RenderPanel
			: public wxPanel
		{
		public:
			RenderPanel( wxWindow * parent, wxSize const & size, Game & game );
			~RenderPanel();
			void reset();
			void setRenderTarget( castor3d::RenderTargetSPtr target );

			castor3d::RenderWindow & getRenderWindow()const
			{
				return *m_renderWindow;
			}

			void disableWindowResize()
			{
				m_resizeWindow = false;
			}

			void enableWindowResize()
			{
				m_resizeWindow = true;
			}

		private:
			float doTransformX( int x );
			float doTransformY( int y );
			int doTransformX( float x );
			int doTransformY( float y );
			void doStartTimer( TimerID id );
			void doStopTimer( TimerID id );

			DECLARE_EVENT_TABLE()
			void onSize( wxSizeEvent & event );
			void onMove( wxMoveEvent & event );
			void onPaint( wxPaintEvent & event );
			void onSetFocus( wxFocusEvent & event );
			void onKillFocus( wxFocusEvent & event );
			void onKeyDown( wxKeyEvent & event );
			void onKeyUp( wxKeyEvent & event );
			void onMouseLDown( wxMouseEvent & event );
			void onMouseLUp( wxMouseEvent & event );
			void onMouseRDown( wxMouseEvent & event );
			void onMouseRUp( wxMouseEvent & event );
			void onMouseMove( wxMouseEvent & event );
			void onMouseWheel( wxMouseEvent & event );
			void onMouseTimer( wxTimerEvent & event );
			void onTimerUp( wxTimerEvent & event );
			void onTimerDown( wxTimerEvent & event );
			void onTimerLeft( wxTimerEvent & event );
			void onTimerRight( wxTimerEvent & event );

		private:
			float m_x{ 0.0 };
			float m_y{ 0.0 };
			float m_oldX{ 0.0 };
			float m_oldY{ 0.0 };
			float m_camSpeed{ 10.0f };
			bool m_mouseLeftDown{ false };
			bool m_resizeWindow{ true };
			std::array< wxTimer *, size_t( TimerID::eCount ) > m_timers;
			NodeStatePtr m_cameraState;
			castor3d::RenderWindowUPtr m_renderWindow;
			castor3d::FrameListenerSPtr m_listener;
			castor3d::CameraWPtr m_camera;
			castor3d::SceneRPtr m_scene{};
			castor3d::GeometryWPtr m_selectedGeometry;
			Game & m_game;
			Entity m_selectedTower;
		};
	}
}
