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

DIRECTORIES = .. ../../QtViewer

# Input
HEADERS += $$getFilesFromDir($$DIRECTORIES,*.hh)
SOURCES += $$getFilesFromDir($$DIRECTORIES,*.cc)
SOURCES -= ../../QtViewer/meshviewer.cc ../subdivider.cc ../adaptive_subdivider.cc
FORMS   += $$getFilesFromDir($$DIRECTORIES,*.ui)

################################################################################
