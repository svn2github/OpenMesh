################################################################################
#
################################################################################

contains( OPENFLIPPER , OpenFlipper ){
	include( $$TOPDIR/qmake/all.include )
} else {
	include( $$TOPDIR/OpenMesh/qmake/all.include )
}

Library()

DIRECTORIES = . Geometry IO IO/exporter IO/importer IO/reader IO/writer \
		Mesh Mesh/gen System Utils

INCLUDEPATH += ../..

win32 {
 DEFINES += _USE_MATH_DEFINES NOMINMAX
 CONFIG  += static
 contains( OPENFLIPPER , OpenFlipper ){
   DESTDIR = $${TOPDIR}/OpenFlipper/$${BUILDDIRECTORY}
   message(OpenMesh Library is Building for OpenFlipper )
 }
}

# Input
HEADERS += $$getFilesFromDir($$DIRECTORIES,*.hh)
SOURCES += $$getFilesFromDir($$DIRECTORIES,*.cc)
FORMS   += $$getFilesFromDir($$DIRECTORIES,*.ui)

################################################################################
