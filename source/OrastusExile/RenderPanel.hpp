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
			RenderPanel( wxWindow * p_parent, wxSize const & p_size, Game & p_game );
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
			void doUpdateSelectedGeometry( castor3d::GeometrySPtr p_geometry );
			void doStartTimer( TimerID p_id );
			void doStopTimer( TimerID p_id );

			DECLARE_EVENT_TABLE()
			void onSize( wxSizeEvent & p_event );
			void onMove( wxMoveEvent & p_event );
			void onPaint( wxPaintEvent & p_event );
			void onSetFocus( wxFocusEvent & p_event );
			void onKillFocus( wxFocusEvent & p_event );
			void onKeyDown( wxKeyEvent & p_event );
			void onKeyUp( wxKeyEvent & p_event );
			void onMouseLDown( wxMouseEvent & p_event );
			void onMouseLUp( wxMouseEvent & p_event );
			void onMouseRDown( wxMouseEvent & p_event );
			void onMouseRUp( wxMouseEvent & p_event );
			void onMouseMove( wxMouseEvent & p_event );
			void onMouseWheel( wxMouseEvent & p_event );
			void onMouseTimer( wxTimerEvent & p_event );
			void onTimerUp( wxTimerEvent & p_event );
			void onTimerDown( wxTimerEvent & p_event );
			void onTimerLeft( wxTimerEvent & p_event );
			void onTimerRight( wxTimerEvent & p_event );

		private:
			float m_x{ 0.0 };
			float m_y{ 0.0 };
			float m_oldX{ 0.0 };
			float m_oldY{ 0.0 };
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
