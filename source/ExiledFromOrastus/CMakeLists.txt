project( ExiledFromOrastus )

set( ${PROJECT_NAME}_WXWIDGET 1 )
set( ${PROJECT_NAME}_DESCRIPTION "Exiled From Orastus" )
set( ${PROJECT_NAME}_VERSION_MAJOR	1 )
set( ${PROJECT_NAME}_VERSION_MINOR	0 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

if( NOT WIN32 )
	find_package( GTK2 )
else()
	set( GTK2_FOUND TRUE )
endif()

find_package( wxWidgets COMPONENTS core base aui adv )

if( wxWidgets_FOUND AND GTK2_FOUND )
	include_directories( ${CMAKE_SOURCE_DIR}/GameEngine )
	include_directories( ${wxWidgets_INCLUDE_DIRS} )
	string( COMPARE EQUAL "${wxWidgets_USE_DEBUG}" "ON" IsWxDebug )
	set( wxWidgets_CXXFLAGS "${wxWidgets_CXX_FLAGS} -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES" )

	if( NOT WIN32 )
		set( wxWidgets_CXXFLAGS "${wxWidgets_CXXFLAGS} -D__WXGTK__" )
	endif()

	if( IsWxDebug )
		set( wxWidgets_CXXFLAGS "${wxWidgets_CXXFLAGS} -D__WXDEBUG__" )
	endif()
	set( wxWidgetsLibraries "" )
	foreach( Lib ${wxWidgets_LIBRARIES} )
		if( wxWidgetsLibraries )
			set( wxWidgetsLibraries "${wxWidgetsLibraries}|${Lib}" )
		else()
			set( wxWidgetsLibraries "${Lib}" )
		endif()
	endforeach()
	foreach( Lib ${GTK2_LIBRARIES} )
		if( wxWidgetsLibraries )
			set( wxWidgetsLibraries "${wxWidgetsLibraries}|${Lib}" )
		else()
			set( wxWidgetsLibraries "${Lib}" )
		endif()
	endforeach()

	add_target(
		${PROJECT_NAME}
		bin
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}
		"GameEngine"
		"${OrastusMinLibraries};${wxWidgetsLibraries}"
		"ExiledFromOrastusPch.hpp"
		"ExiledFromOrastusPch.cpp"
		"${wxWidgets_CFLAGS}"
		"${wxWidgets_CXXFLAGS}"
	)
	set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20 )
	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Outputs" )
	add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )
else()
	if( NOT wxWidgets_FOUND )
		message( WARNING "wxWidgets libraries not found" )
	else()
		message( WARNING "GTK2 libraries not found" )
	endif()
endif ()