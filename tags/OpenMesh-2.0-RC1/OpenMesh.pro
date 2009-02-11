contains( OPENFLIPPER , OpenFlipper ){
	include( $$TOPDIR/qmake/all.include )
} else {
	include( $$TOPDIR/OpenMesh/qmake/all.include )
}

Subdirs()

addSubdirs( Core )
addSubdirs( Tools , Core )
addSubdirs( OpenMesh/Apps/commandlineDecimater , Core Tools)
addSubdirs( Apps/Decimating/DecimaterGui , Core Tools)
addSubdirs( Apps/mconvert , Core Tools)
addSubdirs( Apps/QtViewer , Core Tools)
addSubdirs( Apps/Smoothing , Core Tools)
addSubdirs( Apps/Subdivider/commandlineSubdivider , Core Tools)
addSubdirs( Apps/Subdivider/commandlineAdaptiveSubdivider , Core Tools)
addSubdirs( Apps/Subdivider/SubdividerGui , Core Tools)

win32 {
	!ReleaseBuild{
		!DebugBuild {
			MAKECMD = c:\qt4\bin\qmake.exe

 			CORELIST += Core Tools Apps/Decimating/commandlineDecimater Apps/Decimating/DecimaterGui 
                  CORELIST += Apps/mconvert Apps/QtViewer Apps/Smoothing
			CORELIST += Apps/Subdivider/SubdividerGui Apps/Subdivider/commandlineSubdivider Apps/Subdivider/commandlineAdaptiveSubdivider 

			for(entry, CORELIST ): {
				message( $$entry )
				system( cd $$entry && $$MAKECMD  )			
			}
		}
	}
}

