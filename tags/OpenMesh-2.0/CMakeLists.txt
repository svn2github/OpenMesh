cmake_minimum_required (VERSION 2.6)

project (OpenMesh)

# add our macro directory to cmake search path
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# include our cmake files
include (ACGCommon)

acg_get_version ()

include(OpenMeshPackage)
include (ACGOutput)

if (WIN32)
  add_definitions(
      -D_USE_MATH_DEFINES -DNOMINMAX
  )
endif ()

add_subdirectory (src/OpenMesh/Core)
add_subdirectory (src/OpenMesh/Tools)
add_subdirectory (src/OpenMesh/Apps)
add_subdirectory (Doc)

# ========================================================================
# Bundle generation
# ========================================================================

if (WIN32 )
  # prepare bundle generation cmake file and add a build target for it
  configure_file ("${CMAKE_SOURCE_DIR}/cmake/fixbundle.cmake.win.in"
     "${CMAKE_BINARY_DIR}/fixbundle.win.cmake" @ONLY IMMEDIATE)

  add_custom_target (fixbundle ALL
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_BINARY_DIR}/fixbundle.win.cmake" )

  # let bundle generation depend on all targets
  add_dependencies (fixbundle QtViewer DecimaterGui)
 
endif(WIN32)


if (APPLE)
  # prepare bundle generation cmake file and add a build target for it
  configure_file ("${CMAKE_SOURCE_DIR}/cmake/fixbundle.cmake.in"
     "${CMAKE_BINARY_DIR}/fixbundle.cmake" @ONLY IMMEDIATE)
  add_custom_target (fixbundle ALL
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_BINARY_DIR}/fixbundle.cmake"
  )
  # let bundle generation depend on all targets
  add_dependencies (fixbundle DecimaterGui ProgViewer QtViewer SubdividerGui) 


  # Required for Snow leopard, and the latest qt. Then the resources have to be copied
  if ( EXISTS "/opt/local/libexec/qt4-mac/lib/QtGui.framework/Versions/4/Resources/qt_menu.nib" )
      add_custom_command(TARGET OpenMesh POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory "/opt/local/libexec/qt4-mac/lib/QtGui.framework/Versions/4/Resources/qt_menu.nib" "${CMAKE_BINARY_DIR}/Build/Libraries/qt_menu.nib" 
        )
  endif ()
  
  # install bundle
  #install (DIRECTORY ${CMAKE_BINARY_DIR}/Build/OpenMesh.app DESTINATION .
  #         USE_SOURCE_PERMISSIONS)
endif ()

# ========================================================================


# display results
acg_print_configure_header (OPENMESH "OpenMesh")
