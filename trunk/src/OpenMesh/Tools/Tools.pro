################################################################################
#
################################################################################

contains( OPENFLIPPER , OpenFlipper ){
	include( $$TOPDIR/qmake/all.include )
} else {
	include( $$TOPDIR/OpenMesh/qmake/all.include )
}

Library()

DIRECTORIES = . Decimater Smoother Subdivider/Adaptive/Composite \
        	Subdivider/Uniform/Composite Subdivider/Uniform \
		Utils

INCLUDEPATH += ../..

DESTDIR = $${TOPDIR}/lib

CONFIG( debug, debug|release ){
        TARGET    = OpenMeshToolsd
} else {
        TARGET    = OpenMeshTools
}



win32 {
 DEFINES += _USE_MATH_DEFINES NOMINMAX
 CONFIG  += static
}

macx {
      # Set library binary header to the correct path
      QMAKE_LFLAGS_SONAME = -install_name$${LITERAL_WHITESPACE}$${TOPDIR}/lib/$${BUILDDIRECTORY}/
      export(QMAKE_LFLAGS_SONAME)
}

# Input
HEADERS += $$getFilesFromDir($$DIRECTORIES,*.hh)
SOURCES += $$getFilesFromDir($$DIRECTORIES,*.c)
SOURCES += $$getFilesFromDir($$DIRECTORIES,*.cc) 
FORMS   += $$getFilesFromDir($$DIRECTORIES,*.ui)


################################################################################
