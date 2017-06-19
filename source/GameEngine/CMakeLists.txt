project( GameEngine )

set( ${PROJECT_NAME}_DESCRIPTION "Exiled From Orastus game engine" )
set( ${PROJECT_NAME}_VERSION_MAJOR	1 )
set( ${PROJECT_NAME}_VERSION_MINOR	0 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

set( BinsDependencies
	${BinsDependencies}
	${PROJECT_NAME}
	PARENT_SCOPE
)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR}/Src )
include_directories( ${CMAKE_BINARY_DIR}/Core/GameEngine/Src )

add_target(
	${PROJECT_NAME}
	api_dll
	""
	"${OrastusMinLibraries}"
	"GameEnginePch.hpp"
	"GameEnginePch.cpp"
)

file( GLOB ${PROJECT_NAME}_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*.inl
)
file( GLOB ${PROJECT_NAME}_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp
)
source_group( "Header Files" FILES ${${PROJECT_NAME}_HEADER_FILES} )
source_group( "Source Files" FILES ${${PROJECT_NAME}_SOURCE_FILES} )

set( OrastusMinLibraries
	${OrastusMinLibraries}
	${PROJECT_NAME}
	PARENT_SCOPE
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "GameEngine" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )
target_add_doc( ${PROJECT_NAME} "French" "*.h *.hpp" )
target_add_doc( ${PROJECT_NAME} "English" "*.h *.hpp" )

if ( WIN32 )
	include( InstallRequiredSystemLibraries )
	copy_dll( ${PROJECT_NAME}
		${Castor3D_Castor3D_LIBRARY_DEBUG}
		${Castor3D_Castor3D_LIBRARY_RELEASE} )
	copy_dll( ${PROJECT_NAME}
		${Castor3D_CastorUtils_LIBRARY_DEBUG}
		${Castor3D_CastorUtils_LIBRARY_RELEASE} )
	set( Castor3D_BIN_DIR "${Castor3D_ROOT_DIR}/bin" )
	copy_dll( ${PROJECT_NAME}
		${Castor3D_BIN_DIR}/Debug/assimp-vc140-mtd.dll
		${Castor3D_BIN_DIR}/assimp-vc140-mt.dll )
	copy_dll( ${PROJECT_NAME}
		${Castor3D_BIN_DIR}/Debug/FreeImage.dll
		${Castor3D_BIN_DIR}/FreeImage.dll )
	copy_dll( ${PROJECT_NAME}
		${Castor3D_BIN_DIR}/Debug/zlib.dll
		${Castor3D_BIN_DIR}/zlib.dll )
endif ()

if ( ORASTUS_BUILDGRP_TEST )
	add_subdirectory( Test )
endif ()

