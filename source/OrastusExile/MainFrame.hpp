/**
See licence file in root folder, MIT.txt
*/
#pragma once

#include "OrastusExilePrerequisites.hpp"

#include <wx/frame.h>
#include <wx/windowptr.h>

namespace orastus
{
	namespace main
	{
		class MainFrame
			: public wxFrame
		{
		public:
			MainFrame();
			~MainFrame();

		private:
			void doLoadScene();
			castor3d::RenderTargetSPtr doLoadScene( castor3d::Engine & p_engine
				, castor::Path const & p_fileName
				, uint32_t p_wantedFps
				, bool p_threaded );

			DECLARE_EVENT_TABLE()
			void onPaint( wxPaintEvent  & p_event );
			void onClose( wxCloseEvent  & p_event );
			void onEraseBackground( wxEraseEvent & p_event );
			void onRenderTimer( wxTimerEvent & p_event );
			void onKeyDown( wxKeyEvent & event );
			void onKeyUp( wxKeyEvent & event );
			void OnMouseLdown( wxMouseEvent & event );
			void OnMouseLUp( wxMouseEvent & event );
			void OnMouseRUp( wxMouseEvent & event );
			void OnMouseWheel( wxMouseEvent & event );

		private:
			wxWindowPtr< RenderPanel > m_panel;
			std::unique_ptr< Game > m_game;
			wxTimer * m_timer{ nullptr };
		};
	}
}
