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

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_BINARY_DIR}/Core/GameEngine )

add_target(
	${PROJECT_NAME}
	api_dll
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	""
	"${OrastusMinLibraries}"
	"GameEnginePch.hpp"
	"GameEnginePch.cpp"
)
set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20 )

file( GLOB ${PROJECT_NAME}_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.inl
)
file( GLOB ${PROJECT_NAME}_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
source_group( "Header Files" FILES ${${PROJECT_NAME}_HEADER_FILES} )
source_group( "Source Files" FILES ${${PROJECT_NAME}_SOURCE_FILES} )

parse_subdir_files( ECS "ECS" )
parse_subdir_files( State "State" )

set( OrastusMinLibraries
	${OrastusMinLibraries}
	${PROJECT_NAME}
)
set( OrastusMinLibraries
	${OrastusMinLibraries}
	PARENT_SCOPE
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "GameEngine" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )
target_add_doc( ${PROJECT_NAME} "French" "*.h *.hpp" )
target_add_doc( ${PROJECT_NAME} "English" "*.h *.hpp" )
castor3d_copy_files(
	${PROJECT_NAME}
	${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}
	${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}
	${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}
)

file( GLOB DataFiles
	${CMAKE_CURRENT_SOURCE_DIR}/Data/*.zip
)
copy_target_files_ex( ${PROJECT_NAME}
	ExiledFromOrastus
	""
	${DataFiles}
)
