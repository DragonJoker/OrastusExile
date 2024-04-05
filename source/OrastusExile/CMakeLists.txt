project( OrastusExile )

set( ${PROJECT_NAME}_WXWIDGET 1 )
set( ${PROJECT_NAME}_DESCRIPTION "Orastus' Exile" )
set( ${PROJECT_NAME}_VERSION_MAJOR	1 )
set( ${PROJECT_NAME}_VERSION_MINOR	0 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

if( APPLE )
	set( GTK_FOUND TRUE )
	set( GTK_LIBRARIES
		"-framework AppKit"
		"-framework QuartzCore"
		"-framework Metal"
	)
elseif ( WIN32 )
	set( GTK_FOUND TRUE )
else()
	find_package( GTK3 QUIET )
	find_package( X11 QUIET )
	if ( GTK3_FOUND )
		set( GTK_FOUND ${GTK3_FOUND} )
		set( GTK_INCLUDE_DIRS ${GTK3_INCLUDE_DIRS} )
		set( GTK_LIBRARIES ${GTK3_LIBRARIES} )
	else ()
		set( GTK_FOUND FALSE )
	endif ()
endif()
find_package( wxWidgets COMPONENTS core base aui adv )

if( wxWidgets_FOUND AND GTK_FOUND )
	set( ${PROJECT_NAME}_HDR_FILES
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ImagesLoader.hpp
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/MainFrame.hpp
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/NodeState.hpp
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/OrastusExile.hpp
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/OrastusExilePch.hpp
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/OrastusExilePrerequisites.hpp
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/RenderPanel.hpp
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ImagesLoader.cpp
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/MainFrame.cpp
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/NodeState.cpp
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/OrastusExile.cpp
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/OrastusExilePch.cpp
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/OrastusExilePrerequisites.cpp
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/RenderPanel.cpp
	)
	source_group( "Header Files"
		FILES
			${${PROJECT_NAME}_HDR_FILES}
	)
	source_group( "Source Files"
		FILES
			${${PROJECT_NAME}_SRC_FILES}
	)
	add_target_min(
		${PROJECT_NAME}
		bin
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/${PROJECT_NAME}Pch.hpp
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/${PROJECT_NAME}Pch.cpp
	)
	target_include_directories( ${PROJECT_NAME} SYSTEM
		PRIVATE
			${wxWidgets_INCLUDE_DIRS}
			${GTK_INCLUDE_DIRS}
	)
	target_link_libraries( ${PROJECT_NAME}
		PRIVATE
			orastus::GameEngine
			${wxWidgets_LIBRARIES}
			${GTK_LIBRARIES}
			${X11_LIBRARIES}
	)
	set_target_properties( ${PROJECT_NAME}
		PROPERTIES
			CXX_STANDARD 20
			CXX_EXTENSIONS OFF
			FOLDER "Outputs"
	)
	install_target_ex( ${PROJECT_NAME}
		${MAIN_PROJECT_NAME}
		${MAIN_PROJECT_NAME}
		bin
		${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}
	)
	set( OrastusInstall
		${OrastusInstall}
		${PROJECT_NAME}
		PARENT_SCOPE
	)
	add_target_astyle( ${PROJECT_NAME}
		".h;.hpp;.inl;.cpp"
	)
else()
	if( NOT wxWidgets_FOUND )
		message( WARNING "wxWidgets libraries not found" )
	else()
		message( WARNING "GTK libraries not found" )
	endif()
endif ()