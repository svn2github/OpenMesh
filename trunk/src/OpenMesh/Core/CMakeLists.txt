include (ACGCommon)

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
acg_append_files (headers "*.hh" ${directories})
acg_append_files (sources "*.cc" ${directories})

IF(WIN32)
  acg_append_files (sources "*.c" ${directories})
ENDIF(WIN32)

if (WIN32)
  # OpenMesh has no dll exports so we have to build a static library on windows
  acg_add_library (OpenMeshCore STATIC ${sources} ${headers})
else ()
  acg_add_library (OpenMeshCore SHARED ${sources} ${headers})
endif ()


