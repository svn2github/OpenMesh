if ( NOT DEFINED OPENMESH_BUILD_PYTHON_INTEGRATION)
    set( OPENMESH_BUILD_PYTHON_INTEGRATION false CACHE BOOL "Enable or disable Python integration of OpenMesh." )
endif()

if ( OPENMESH_BUILD_PYTHON_INTEGRATION )
  # TODO: ensure that python libs and boost python libs match up
        FIND_PACKAGE( PythonLibs 3.2 REQUIRED )
        FIND_PACKAGE( Boost 1.46.0 COMPONENTS python-py32 REQUIRED )
	
	include_directories(${PYTHON_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ../)
	file(GLOB SOURCES *.cc)
	add_library(openmesh SHARED
		${SOURCES}
	)
	target_link_libraries(openmesh OpenMeshCore OpenMeshTools ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
	set_target_properties(openmesh PROPERTIES
		PREFIX ""
		LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib/python/acg")
	FILE(INSTALL acg DESTINATION "${PROJECT_BINARY_DIR}/lib/python")

        FILE(COPY UnitTests/testBindings.py DESTINATION "${PROJECT_BINARY_DIR}/lib/python/acg")
endif()

