################################################################################
#
################################################################################

contains( OPENFLIPPER , OpenFlipper ){
	include( $$TOPDIR/qmake/all.include )
} else {
	include( $$TOPDIR/OpenMesh/qmake/all.include )
}

INCLUDEPATH += ../../..

Application()
openmesh()

# Input
SOURCES += ../adaptive_subdivider.cc

################################################################################
