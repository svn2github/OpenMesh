include (common)

include_directories (
  ../..
  ${CMAKE_CURRENT_SOURCE_DIR}
)

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

append_files (headers "*.hh" ${directories})
append_files (sources "*.cc" ${directories})

add_library (OpenMeshCore STATIC ${sources} ${headers})

set_target_props (OpenMeshCore)