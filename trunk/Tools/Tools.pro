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

win32 {
 DEFINES += _USE_MATH_DEFINES NOMINMAX
 CONFIG  += static
}

macx {
      # Post process
      QMAKE_LFLAGS_SONAME = -install_name$${LITERAL_WHITESPACE}$${TOPDIR}/OpenMesh/Tools/lib/$${BUILDDIRECTORY}/
      export(QMAKE_LFLAGS_SONAME)
}

# Input
HEADERS += $$getFilesFromDir($$DIRECTORIES,*.hh)
SOURCES += $$getFilesFromDir($$DIRECTORIES,*.c)
SOURCES += $$getFilesFromDir($$DIRECTORIES,*.cc) 
FORMS   += $$getFilesFromDir($$DIRECTORIES,*.ui)


################################################################################
