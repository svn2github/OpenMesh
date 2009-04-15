include (common)

include_directories (
  ../..
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set (directories 
  . 
  Decimater
  Smoother
  Subdivider/Adaptive/Composite
  Subdivider/Uniform/Composite 
  Subdivider/Uniform
  Utils
)

append_files (headers "*.hh" ${directories})
append_files (sources "*.cc" ${directories})

if (WIN32)
add_library (OpenMeshTools STATIC ${sources} ${headers})
else ()
add_library (OpenMeshTools SHARED ${sources} ${headers})
endif ()

target_link_libraries (OpenMeshTools OpenMeshCore)

set_target_props (OpenMeshTools)
