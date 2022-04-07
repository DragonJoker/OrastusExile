project( GameEngineTest )

set( ${PROJECT_NAME}_DESCRIPTION "${PROJECT_NAME} application" )
set( ${PROJECT_NAME}_VERSION_MAJOR	1 )
set( ${PROJECT_NAME}_VERSION_MINOR	0 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

set( ${PROJECT_NAME}_HDR_FILES
	${CMAKE_SOURCE_DIR}/test/GameEngine/BulletTest.hpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/ComponentDataTest.hpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/ComponentTest.hpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/EcsTest.hpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/EnemyTest.hpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/EntityTest.hpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/GameEngineTestPrerequisites.hpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/ResourceTest.hpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/TowerTest.hpp
)
set( ${PROJECT_NAME}_SRC_FILES
	${CMAKE_SOURCE_DIR}/test/GameEngine/BulletTest.cpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/ComponentDataTest.cpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/ComponentTest.cpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/EcsTest.cpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/EnemyTest.cpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/EntityTest.cpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/main.cpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/ResourceTest.cpp
	${CMAKE_SOURCE_DIR}/test/GameEngine/TowerTest.cpp
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
	bin_dos
	""
	""
)
target_link_libraries( ${PROJECT_NAME}
	PUBLIC
		castor::CastorTest
		orastus::GameEngine
)
set_target_properties( ${PROJECT_NAME}
	PROPERTIES
		CXX_STANDARD 20
		CXX_EXTENSIONS OFF
		FOLDER "Test"
)
install_target_ex( ${PROJECT_NAME}
	${PROJECT_NAME}
	bin
	${CMAKE_SOURCE_DIR}/test/GameEngine
)
set( OrastusInstall
	${OrastusInstall}
	${PROJECT_NAME}
	PARENT_SCOPE
)
add_target_astyle( ${PROJECT_NAME}
	".h;.hpp;.inl;.cpp"
)
add_test( NAME ${PROJECT_NAME}
	COMMAND ${PROJECT_NAME}
)
