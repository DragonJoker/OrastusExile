/**
See licence file in root folder, MIT.txt
*/
#pragma once

#include "ExiledFromOrastusPrerequisites.hpp"

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
			void DoLoadScene();
			Castor3D::RenderWindowSPtr DoLoadScene( Castor3D::Engine & p_engine
				, Castor::Path const & p_fileName
				, uint32_t p_wantedFps
				, bool p_threaded );

			DECLARE_EVENT_TABLE()
			void OnPaint( wxPaintEvent  & p_event );
			void OnClose( wxCloseEvent  & p_event );
			void OnEraseBackground( wxEraseEvent & p_event );
			void OnRenderTimer( wxTimerEvent & p_event );

		private:
			wxWindowPtr< RenderPanel > m_panel;
			std::unique_ptr< Game > m_game;
			wxTimer * m_timer{ nullptr };
		};
	}
}
