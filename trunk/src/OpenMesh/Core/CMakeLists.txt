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

if (WIN32)
  # OpenMesh has no dll exports so we have to build a static library on windows
  acg_add_library (OpenMeshCore STATIC ${sources} ${headers})
else ()
  acg_add_library (OpenMeshCore SHAREDANDSTATIC ${sources} ${headers})
endif ()


# Install Header Files

if (NOT ACG_PROJECT_MACOS_BUNDLE OR NOT APPLE)
install(DIRECTORY . 
	DESTINATION include/OpenMesh/Core
        FILES_MATCHING 
	PATTERN "*.hh"
        PATTERN "CVS" EXCLUDE
        PATTERN ".svn" EXCLUDE
        PATTERN "tmp" EXCLUDE
	PATTERN "Templates" EXCLUDE
        PATTERN "Debian*" EXCLUDE)

#install Template cc files (required by headers)
install(DIRECTORY . 
	DESTINATION include/OpenMesh/Core
        FILES_MATCHING 
	PATTERN "*T.cc"
        PATTERN "CVS" EXCLUDE
        PATTERN ".svn" EXCLUDE
	PATTERN "tmp" EXCLUDE
	PATTERN "Templates" EXCLUDE
        PATTERN "Debian*" EXCLUDE)

#install the config file
install(FILES System/config.h DESTINATION include/OpenMesh/Core/System)

#install inlined Files from IO
install(DIRECTORY IO/ 
	DESTINATION include/OpenMesh/Core/IO
	FILES_MATCHING
	PATTERN "*.inl"         
	PATTERN "CVS" EXCLUDE
        PATTERN ".svn" EXCLUDE
        PATTERN "reader" EXCLUDE
        PATTERN "writer" EXCLUDE
        PATTERN "importer" EXCLUDE
        PATTERN "exporter" EXCLUDE
        PATTERN "tmp" EXCLUDE
        PATTERN "Debian*" EXCLUDE )
endif ()


