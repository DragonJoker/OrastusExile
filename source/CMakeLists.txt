include( FindCastor3D )

add_subdirectory( GameEngine )
add_subdirectory( OrastusExile )

set( OrastusInstall
	${OrastusInstall}
	PARENT_SCOPE
)
