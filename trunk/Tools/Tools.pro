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

# Input
HEADERS += $$getFilesFromDir($$DIRECTORIES,*.hh)
SOURCES += $$getFilesFromDir($$DIRECTORIES,*.c)
SOURCES += $$getFilesFromDir($$DIRECTORIES,*.cc) 
FORMS   += $$getFilesFromDir($$DIRECTORIES,*.ui)


################################################################################
