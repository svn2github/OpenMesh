cmake_minimum_required (VERSION 2.6)

# Only set project name if OpenMesh is built as stand-alone library
if("${PROJECT_NAME}" STREQUAL "")
    project (OpenMesh)
endif()

# add our macro directory to cmake search path
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set (CMAKE_DEBUG_POSTFIX "d")

# include our cmake files
include (ACGCommon)

acg_get_version ()


# Disable package building when built as an external library
if(${PROJECT_NAME} MATCHES "OpenMesh")
    include(OpenMeshPackage)
endif()

include (ACGOutput)

if (WIN32)
  add_definitions(
      -D_USE_MATH_DEFINES -DNOMINMAX
  )
endif ()

# ========================================================================
# Add bundle targets here
# ========================================================================
if ( NOT DEFINED BUILD_APPS )
  set( BUILD_APPS true CACHE BOOL "Enable or disable building of apps" )
endif()

if (WIN32)
  if ( NOT "${CMAKE_GENERATOR}" MATCHES "MinGW Makefiles" AND BUILD_APPS )
    add_custom_target (fixbundle ALL
      COMMAND ${CMAKE_COMMAND} -P "${CMAKE_BINARY_DIR}/fixbundle.win.cmake" )
  endif()
endif()

if (APPLE)
  add_custom_target (fixbundle ALL
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_BINARY_DIR}/fixbundle.cmake"
  )
endif()

# ========================================================================
# Call the subdirectories with there projects
# ========================================================================

add_subdirectory (src/OpenMesh/Core)
add_subdirectory (src/OpenMesh/Tools)
add_subdirectory (src/OpenMesh/Apps)

# Do not build unit tests when build as external library
if(${PROJECT_NAME} MATCHES "OpenMesh")
    add_subdirectory (src/Unittests)
endif()

add_subdirectory (Doc)

# ========================================================================
# Bundle generation (Targets exist, now configure them)
# ========================================================================

if (WIN32 AND  BUILD_APPS )
  # prepare bundle generation cmake file and add a build target for it
  configure_file ("${CMAKE_SOURCE_DIR}/cmake/fixbundle.cmake.win.in"
     "${CMAKE_BINARY_DIR}/fixbundle.win.cmake" @ONLY IMMEDIATE)

  if ( NOT "${CMAKE_GENERATOR}" MATCHES "MinGW Makefiles" )
    # let bundle generation depend on all targets
    add_dependencies (fixbundle QtViewer DecimaterGui)
  endif()
 
endif()


if (APPLE)
  # prepare bundle generation cmake file and add a build target for it
  configure_file ("${CMAKE_SOURCE_DIR}/cmake/fixbundle.cmake.in"
     "${CMAKE_BINARY_DIR}/fixbundle.cmake" @ONLY IMMEDIATE)

  # let bundle generation depend on all targets
  add_dependencies (fixbundle DecimaterGui ProgViewer QtViewer SubdividerGui) 


  # Required for Snow leopard, and the latest qt. Then the resources have to be copied
  if ( EXISTS "/opt/local/libexec/qt4-mac/lib/QtGui.framework/Versions/4/Resources/qt_menu.nib" )
      add_custom_command(TARGET OpenMesh POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory "/opt/local/libexec/qt4-mac/lib/QtGui.framework/Versions/4/Resources/qt_menu.nib" "${CMAKE_BINARY_DIR}/Build/Libraries/qt_menu.nib" 
        )
  endif ()
  
endif ()

# ========================================================================


# display results
acg_print_configure_header (OPENMESH "OpenMesh")
