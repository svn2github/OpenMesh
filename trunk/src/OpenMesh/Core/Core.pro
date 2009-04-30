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


DESTDIR = $${TOPDIR}/lib

CONFIG( debug, debug|release ){
        TARGET    = OpenMeshCored
} else {
	TARGET    = OpenMeshCore
}

win32 {
 DEFINES += _USE_MATH_DEFINES NOMINMAX
 CONFIG  += static
 contains( OPENFLIPPER , OpenFlipper ){
   DESTDIR = $${TOPDIR}/OpenFlipper/$${BUILDDIRECTORY}
   message(OpenMesh Library is Building for OpenFlipper )
 }
}

macx {
      # Set library binary header to the correct path
      QMAKE_LFLAGS_SONAME = -install_name$${LITERAL_WHITESPACE}$${TOPDIR}/OpenMesh/Core/lib/$${BUILDDIRECTORY}/
      export(QMAKE_LFLAGS_SONAME)
}

# Input
HEADERS += $$getFilesFromDir($$DIRECTORIES,*.hh)
SOURCES += $$getFilesFromDir($$DIRECTORIES,*.cc)
FORMS   += $$getFilesFromDir($$DIRECTORIES,*.ui)

################################################################################
