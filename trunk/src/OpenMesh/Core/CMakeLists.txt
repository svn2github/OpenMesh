include (common)

include_directories (
  ../..
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# source code directories
set (directories 
  . 
  Geometry 
  IO 
  IO/exporter 
  IO/importer 
  IO/reader 
  IO/writer
  Mesh 
  Mesh/gen 
  System 
  Utils
)

# collect all header and source files
append_files (headers "*.hh" ${directories})
append_files (sources "*.cc" ${directories})

if (WIN32)
  # OpenMesh has no dll exports so we have to build a static library on windows
  add_library (OpenMeshCore STATIC ${sources} ${headers})
else ()
  add_library (OpenMeshCore SHARED ${sources} ${headers})
  # no install on mac, because the whole bundle will be installed in the
  # toplevel CMakeLists.txt
  if (NOT APPLE)
    install (TARGETS OpenMeshCore DESTINATION ${OPENFLIPPER_LIBDIR})
  endif ()
endif ()

# set common target properties defined in common.cmake
set_target_props (OpenMeshCore)
