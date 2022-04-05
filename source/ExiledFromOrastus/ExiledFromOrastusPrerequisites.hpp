/**
See licence file in root folder, MIT.txt
*/
#pragma once

#include <Castor3D/Castor3DPrerequisites.hpp>
#include <GameEnginePrerequisites.hpp>

#include <wx/wx.h>
#include <wx/windowptr.h>

#include <chrono>
#include <vector>

namespace orastus
{
	namespace main
	{
		class ExiledFromOrastus;
		class MainFrame;
		class RenderPanel;

		struct Config
		{
			bool validate{ false };
			castor::LogType log{ castor::LogType::eInfo };
			castor::String rendererName;
			uint32_t gpuIndex{ 0u };
		};

		wxString const ApplicationName = wxT( "ExiledFromOrastus" );

		template< typename T, typename ... Params >
		inline wxWindowPtr< T > wxMakeWindowPtr( Params && ... params )
		{
			return wxWindowPtr< T >( new T( std::forward< Params >( params )... ) );
		}

		wxString makeWxString( castor::String const & value );
		castor::Size makeSize( wxSize const & size );
		wxSize makeWxSize( castor::Size const & size );
		ashes::WindowHandle makeWindowHandle( wxWindow * window );

		static const wxColour PANEL_BACKGROUND_COLOUR = wxColour( 30, 30, 30 );
		static const wxColour PANEL_FOREGROUND_COLOUR = wxColour( 220, 220, 220 );
		static const wxColour BORDER_COLOUR = wxColour( 90, 90, 90 );
		static const wxColour INACTIVE_TAB_COLOUR = wxColour( 60, 60, 60 );
		static const wxColour INACTIVE_TEXT_COLOUR = wxColour( 200, 200, 200 );
		static const wxColour ACTIVE_TAB_COLOUR = wxColour( 51, 153, 255, 255 );
		static const wxColour ACTIVE_TEXT_COLOUR = wxColour( 255, 255, 255, 255 );
	}
}
