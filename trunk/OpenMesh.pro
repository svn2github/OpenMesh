contains( OPENFLIPPER , OpenFlipper ){
	include( $$TOPDIR/qmake/all.include )
} else {
	include( $$TOPDIR/OpenMesh/qmake/all.include )
}

Subdirs()

addSubdirs( Core )
addSubdirs( Tools , Core )

addSubdirs( Apps/Decimating , Core Tools)
addSubdirs( Apps/mconvert , Core Tools)
addSubdirs( Apps/QtViewer , Core Tools)
addSubdirs( Apps/Smoothing , Core Tools)
addSubdirs( Apps/Subdivider , Core Tools)

win32 {
	!ReleaseBuild{
		!DebugBuild {
			MAKECMD = c:\qt4\bin\qmake.exe

 			CORELIST += Core Tools Apps/Decimating Apps/mconvert Apps/QtViewer Apps/Smoothing Apps/Subdivider

			for(entry, CORELIST ): {
				message( $$entry )
				system( cd $$entry && $$MAKECMD  )			
			}
		}
	}
}

