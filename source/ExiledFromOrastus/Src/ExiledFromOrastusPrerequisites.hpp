/**
See licence file in root folder, MIT.txt
*/
#pragma once

#include <Castor3DPrerequisites.hpp>
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

		struct Cell;
		class Grid;
		class Game;
		class Enemy;
		class Bullet;
		class Game;

		using EnemyPtr = std::shared_ptr< Enemy >;

		using EnemyArray = std::vector< EnemyPtr >;
		using BulletArray = std::vector< Bullet >;

		using Clock = std::chrono::high_resolution_clock;

		wxString const ApplicationName = wxT( "ExiledFromOrastus" );

		template< typename T, typename ... Params >
		inline wxWindowPtr< T > wxMakeWindowPtr( Params && ... p_params )
		{
			return wxWindowPtr< T >( new T( std::forward< Params >( p_params )... ) );
		}

		Castor::Size MakeSize( wxSize const & p_size );
		Castor3D::WindowHandle MakeWindowHandle( wxWindow * p_window );

		static const wxColour PANEL_BACKGROUND_COLOUR = wxColour( 30, 30, 30 );
		static const wxColour PANEL_FOREGROUND_COLOUR = wxColour( 220, 220, 220 );
		static const wxColour BORDER_COLOUR = wxColour( 90, 90, 90 );
		static const wxColour INACTIVE_TAB_COLOUR = wxColour( 60, 60, 60 );
		static const wxColour INACTIVE_TEXT_COLOUR = wxColour( 200, 200, 200 );
		static const wxColour ACTIVE_TAB_COLOUR = wxColour( 51, 153, 255, 255 );
		static const wxColour ACTIVE_TEXT_COLOUR = wxColour( 255, 255, 255, 255 );
	}
}
