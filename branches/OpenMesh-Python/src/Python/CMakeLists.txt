IF(NOT DEFINED OPENMESH_BUILD_PYTHON_INTEGRATION)
	SET(OPENMESH_BUILD_PYTHON_INTEGRATION FALSE CACHE BOOL "Enable or disable building OpenMesh for Python.")
ENDIF()

IF(NOT DEFINED OPENMESH_PYTHON_VERSION)
	SET(OPENMESH_PYTHON_VERSION "2.7" CACHE STRING "Choose the Python version that is used to build OpenMesh for Python.")
ENDIF()

IF(OPENMESH_BUILD_PYTHON_INTEGRATION)
	# Find python lib
	FIND_PACKAGE(PythonLibs ${OPENMESH_PYTHON_VERSION} REQUIRED)

	# Determine the name of the python component
	STRING(REGEX MATCH "^[0-9]+\\.[0-9]+" PYTHON_VERSION ${PYTHONLIBS_VERSION_STRING})
	STRING(REGEX REPLACE "\\." "" PYTHON_VERSION ${PYTHON_VERSION})

	SET(BOOST_PYTHON_COMPONENT "python-py${PYTHON_VERSION}")

	# Check known boost versions for the python component
	SET(BOOST_KNOWN_VERSIONS
		"1.56.0" "1.56" "1.55.0" "1.55" "1.54.0" "1.54"
		"1.53.0" "1.53" "1.52.0" "1.52" "1.51.0" "1.51"
		"1.50.0" "1.50" "1.49.0" "1.49" "1.48.0" "1.48" "1.47.0" "1.47" "1.46.1"
		"1.46.0" "1.46" "1.45.0" "1.45" "1.44.0" "1.44" "1.43.0" "1.43" "1.42.0" "1.42"
		"1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37"
		"1.36.1" "1.36.0" "1.36" "1.35.1" "1.35.0" "1.35" "1.34.1" "1.34.0"
		"1.34" "1.33.1" "1.33.0" "1.33"
	)
	
	SET(Boost_FIND_QUIETLY TRUE)
	
	MESSAGE(STATUS "Looking for Boost Python...")
	
	FOREACH(VERSION ${BOOST_KNOWN_VERSIONS})
		FIND_PACKAGE(Boost ${VERSION} EXACT COMPONENTS ${BOOST_PYTHON_COMPONENT})
		IF(Boost_FOUND)
			BREAK()
		ENDIF(Boost_FOUND)
	ENDFOREACH()

	# Check if boost was found
	IF(NOT Boost_FOUND)
		MESSAGE(FATAL_ERROR "Could not find Boost Python.")
	ENDIF(NOT Boost_FOUND)


	include_directories(${PYTHON_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ../)
	
	file(GLOB SOURCES *.cc *hh)
	add_library(openmesh SHARED ${SOURCES})
	
	target_link_libraries(openmesh OpenMeshCore OpenMeshTools ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
	set_target_properties(openmesh PROPERTIES
		PREFIX ""
		LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib/python/acg")
	FILE(INSTALL acg DESTINATION "${PROJECT_BINARY_DIR}/lib/python")

	FILE(GLOB UNITTESTS UnitTests/*.py)
	FOREACH(TEST ${UNITTESTS})
		FILE(COPY ${TEST} DESTINATION "${PROJECT_BINARY_DIR}/lib/python/acg")
	ENDFOREACH()
endif()
