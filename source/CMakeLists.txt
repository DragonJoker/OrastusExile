include( FindCastor3D )

add_subdirectory( GameEngine )
add_subdirectory( ExiledFromOrastus )

set( OrastusInstall
	${OrastusInstall}
	PARENT_SCOPE
)
