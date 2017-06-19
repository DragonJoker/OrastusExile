project( GameEngineTest )

include_directories( ${CMAKE_SOURCE_DIR}/Core/GameEngine/Src )
include_directories( ${CMAKE_SOURCE_DIR}/Core/GameEngineTest/Src )

set( ${PROJECT_NAME}_DESCRIPTION "${PROJECT_NAME} application" )
set( ${PROJECT_NAME}_VERSION_MAJOR	1 )
set( ${PROJECT_NAME}_VERSION_MINOR	0 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

add_target(
	${PROJECT_NAME}
	bin_dos
	"GameEngine"
	"${OrastusMinLibraries}"
)

target_link_libraries( ${PROJECT_NAME} ${Castor3D_CastorTest_LIBRARIES} )

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Test" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

add_test( NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME} )

set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
