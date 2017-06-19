#include "ExiledFromOrastusPrerequisites.hpp"

#if defined( CASTOR_PLATFORM_WINDOWS  ) && !defined( NDEBUG ) && !defined( VLD_AVAILABLE )
#	define _CRTDBG_MAP_ALLOC
#	include <cstdlib>
#	include <crtdbg.h>
#elif defined( CASTOR_PLATFORM_LINUX )
#	include <gdk/gdkx.h>
#	include <gtk/gtk.h>
#endif

#include <Miscellaneous/PlatformWindowHandle.hpp>
#include <Engine.hpp>

namespace orastus
{
	namespace main
	{
		Castor::Size MakeSize( wxSize const & p_size )
		{
			return Castor::Size{ uint32_t( p_size.x ), uint32_t( p_size.y ) };
		}

		Castor3D::WindowHandle MakeWindowHandle( wxWindow * p_window )
		{
#if defined( CASTOR_PLATFORM_WINDOWS )

			return Castor3D::WindowHandle( std::make_shared< Castor3D::IMswWindowHandle >( p_window->GetHandle() ) );

#elif defined( CASTOR_PLATFORM_LINUX )

			GtkWidget * l_gtkWidget = static_cast< GtkWidget * >( p_window->GetHandle() );
			GLXDrawable l_drawable = 0;
			Display * l_display = nullptr;

			if ( l_gtkWidget && l_gtkWidget->window )
			{
				l_drawable = GDK_WINDOW_XID( l_gtkWidget->window );
				GdkDisplay * l_gtkDisplay = gtk_widget_get_display( l_gtkWidget );

				if ( l_gtkDisplay )
				{
					l_display = gdk_x11_display_get_xdisplay( l_gtkDisplay );
				}
			}

			return Castor3D::WindowHandle( std::make_shared< IXWindowHandle >( l_drawable, l_display ) );

#endif
		}
	}
}
