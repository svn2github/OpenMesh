################################################################################
#
################################################################################

contains( OPENFLIPPER , OpenFlipper ){
        include( $$TOPDIR/qmake/all.include )
} else {
        include( $$TOPDIR/OpenMesh/qmake/all.include )
}

Subdirs()
SUBDIRS  = commandline commandlineAdaptive  gui  


################################################################################
