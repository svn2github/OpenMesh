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
glew()
glut()
openmesh()

DIRECTORIES = .. 

# Input
SOURCES += ../decimater.cc

################################################################################
