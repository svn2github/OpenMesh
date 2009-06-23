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

IF(WIN32)
  acg_append_files (sources "*.c" ${directories})
ENDIF(WIN32)

if (WIN32)
  # OpenMesh has no dll exports so we have to build a static library on windows
  acg_add_library (OpenMeshTools STATIC ${sources} ${headers})
else ()
  acg_add_library (OpenMeshTools SHAREDANDSTATIC ${sources} ${headers})
  set_target_properties (OpenMeshTools PROPERTIES VERSION ${OPENMESH_VERSION_MAJOR}.${OPENMESH_VERSION_MINOR}
                                                SOVERSION ${OPENMESH_VERSION_MAJOR}.${OPENMESH_VERSION_MINOR} )
endif ()

target_link_libraries (OpenMeshTools OpenMeshCore)

IF( NOT WIN32 )
  target_link_libraries (OpenMeshToolsStatic OpenMeshCoreStatic)
ENDIF(NOT WIN32)

if (NOT ACG_PROJECT_MACOS_BUNDLE OR NOT APPLE)
# Install Header Files
install(DIRECTORY .
        DESTINATION include/OpenMesh/Tools
        FILES_MATCHING
        PATTERN "*.hh"
        PATTERN "CVS" EXCLUDE
        PATTERN ".svn" EXCLUDE
        PATTERN "tmp" EXCLUDE
        PATTERN "Templates" EXCLUDE
        PATTERN "Debian*" EXCLUDE)

#install Template cc files (required by headers)
install(DIRECTORY .
        DESTINATION include/OpenMesh/Tools
        FILES_MATCHING
        PATTERN "*T.cc"
        PATTERN "CVS" EXCLUDE
        PATTERN ".svn" EXCLUDE
        PATTERN "tmp" EXCLUDE
        PATTERN "Templates" EXCLUDE
        PATTERN "Debian*" EXCLUDE)
endif ()

#install the config file
install(FILES Utils/getopt.h DESTINATION include/OpenMesh/Tools/Utils)


