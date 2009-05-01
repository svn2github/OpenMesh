include (ACGCommon)

include_directories (
  ../..
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# source code directories
set (directories 
  . 
  Decimater
  Smoother
  Subdivider/Adaptive/Composite
  Subdivider/Uniform/Composite 
  Subdivider/Uniform
  Utils
)

# collect all header and source files
acg_append_files (headers "*.hh" ${directories})
acg_append_files (sources "*.cc" ${directories})

if (WIN32)
  # OpenMesh has no dll exports so we have to build a static library on windows
  acg_add_library (OpenMeshTools STATIC ${sources} ${headers})
else ()
  acg_add_library (OpenMeshTools SHARED ${sources} ${headers})
endif ()

target_link_libraries (OpenMeshTools OpenMeshCore)

