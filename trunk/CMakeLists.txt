cmake_minimum_required (VERSION 2.6)

project (OpenMesh)

# add our macro directory to cmake search path
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# include our cmake files
include (ACGCommon)

acg_get_version ()

include (ACGOutput)

if (WIN32)
  add_definitions(
      -D_USE_MATH_DEFINES -DNOMINMAX
  )
endif ()

add_subdirectory (src/OpenMesh/Core)
add_subdirectory (src/OpenMesh/Tools)
add_subdirectory (src/OpenMesh/Apps)

# display results
acg_print_configure_header ("OpenMesh")
