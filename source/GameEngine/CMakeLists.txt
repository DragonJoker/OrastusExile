project( GameEngine )

set( ${PROJECT_NAME}_DESCRIPTION "Exiled From Orastus game engine" )
set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_BUILD 0 )

set( ${PROJECT_NAME}_HDR_FILES
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/Ability.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/BulletSpawner.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/EnemySpawner.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/Game.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/GameEnginePch.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/GameEnginePrerequisites.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/Grid.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/GridCell.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/GridPathNode.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/Hud.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/Resource.hpp
)
set( ${PROJECT_NAME}_SRC_FILES
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/BulletSpawner.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/EnemySpawner.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/Game.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/GameEnginePch.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/Grid.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/Hud.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/Resource.cpp
)
source_group( "Header Files"
	FILES
		${${PROJECT_NAME}_HDR_FILES}
)
source_group( "Source Files"
	FILES
		${${PROJECT_NAME}_SRC_FILES}
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/AnimationData.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/AttackData.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/Bullet.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/Component.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/ComponentData.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/Ecs.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/Ecs.inl
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/Enemy.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/Entity.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/EntityComponentData.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/Id.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/MapBlock.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/SplashTower.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/StateSystem.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/Tower.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/TrackData.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/ECS/WalkData.hpp
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ECS/AnimationData.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ECS/AttackData.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ECS/Bullet.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ECS/Component.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ECS/Ecs.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ECS/Enemy.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ECS/Entity.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ECS/Id.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ECS/MapBlock.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ECS/SplashTower.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ECS/StateSystem.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ECS/Tower.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ECS/TrackData.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/ECS/WalkData.cpp
)
source_group( "Header Files\\ECS"
	FILES
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
source_group( "Source Files\\ECS"
	FILES
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
set( ${PROJECT_NAME}_FOLDER_HDR_FILES
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/State/TowerState.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/State/BulletState.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/State/EnemyState.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/State/State.hpp
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/State/StateMachine.hpp
)
set( ${PROJECT_NAME}_FOLDER_SRC_FILES
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/State/TowerState.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/State/BulletState.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/State/EnemyState.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/State/State.cpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/State/StateMachine.cpp
)
source_group( "Header Files\\State"
	FILES
		${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
source_group( "Source Files\\State"
	FILES
		${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${${PROJECT_NAME}_FOLDER_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	${${PROJECT_NAME}_SRC_FILES}
	${${PROJECT_NAME}_FOLDER_SRC_FILES}
)
add_target_min(
	${PROJECT_NAME}
	api_dll
	${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}Pch.hpp
	${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/${PROJECT_NAME}Pch.cpp
)
add_library( orastus::${PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:include>
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		ashes::ashesCommon
		ashes::ashes
		ashes::ashespp
		ashes::ashesVkRenderer
		crg::RenderGraph
		sdw::ShaderAST
		sdw::ShaderWriter
		sdw::CompilerSpirV
	PUBLIC
		castor::CastorUtils
		castor::Castor3D
)
set_target_properties( ${PROJECT_NAME}
	PROPERTIES
		CXX_STANDARD 20
		CXX_EXTENSIONS OFF
		FOLDER "GameEngine"
)
install_target_ex( ${PROJECT_NAME}
	${PROJECT_NAME}
	bin
	${CASTOR_SOURCE_DIR}/include/${PROJECT_NAME}
)
set( OrastusInstall
	${OrastusInstall}
	${PROJECT_NAME}
	PARENT_SCOPE
)
add_target_astyle( ${PROJECT_NAME}
	".h;.hpp;.inl;.cpp"
)
target_add_doc( ${PROJECT_NAME}
	"French"
	"*.h *.hpp"
)
target_add_doc( ${PROJECT_NAME}
	"English"
	"*.h *.hpp"
)
castor3d_copy_files(
	${PROJECT_NAME}
	${PROJECTS_BINARIES_OUTPUT_DIR_RELEASE}
	${PROJECTS_BINARIES_OUTPUT_DIR_RELWITHDEBINFO}
	${PROJECTS_BINARIES_OUTPUT_DIR_DEBUG}
)
file( GLOB DataFiles
	${CMAKE_SOURCE_DIR}/data/${PROJECT_NAME}/*.zip
)
copy_target_files_ex( ${PROJECT_NAME}
	${PROJECT_NAME}
	""
	${DataFiles}
)
