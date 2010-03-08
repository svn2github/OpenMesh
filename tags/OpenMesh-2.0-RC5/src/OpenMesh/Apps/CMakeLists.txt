include (ACGCommon)

# find needed packages
find_package (OpenGL)
find_package (GLUT)
find_package (GLEW)

acg_qt4 ()

add_subdirectory (Decimating/commandlineDecimater)
add_subdirectory (Smoothing)
add_subdirectory (Subdivider/commandlineSubdivider)
add_subdirectory (Subdivider/commandlineAdaptiveSubdivider)
add_subdirectory (mconvert)
add_subdirectory (VDProgMesh/mkbalancedpm)
add_subdirectory (VDProgMesh/Analyzer)

# check for OpenGL, GLEW and GLUT as our required dependencies
if (QT4_FOUND AND OPENGL_FOUND AND GLEW_FOUND AND GLUT_FOUND AND NOT BUILD_APPS STREQUAL OFF  )
  add_subdirectory (Decimating/DecimaterGui)
  add_subdirectory (QtViewer)
  add_subdirectory (Subdivider/SubdividerGui)
  add_subdirectory (ProgViewer)
  add_subdirectory (VDProgMesh/Synthesizer)
else ()
  if ( BUILD_APPS STREQUAL OFF )
    message ("Building Apps disabled by user.")
  endif ()

  if (NOT QT4_FOUND)
    message ("QT 4 not found! Skipping some apps.")
  endif ()

  if (NOT OPENGL_FOUND)
    message ("OpengGL not found! Skipping some apps.")
  endif ()

  if (NOT GLEW_FOUND)
    message ("GLEW not found! Skipping some apps.")
  endif ()

  if (NOT GLUT_FOUND)
    message ("GLUT not found! Skipping some apps.")
  endif ()
endif ()
