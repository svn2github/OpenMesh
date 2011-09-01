include (ACGCommon)

include_directories (
  ..
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Search for gtest headers and libraries
find_package(GoogleTest)

if(GTEST_FOUND)
   
    # Set correct include paths so that the compiler can find the headers
    include_directories(${GTEST_INCLUDE_DIRS})
    # Create new target named unittests_hexmeshing
    add_executable(unittests EXCLUDE_FROM_ALL unittests.cc)
    # Link against all necessary libraries
    target_link_libraries(unittests OpenMeshCore OpenMeshTools gtest gtest_main pthread)
    # Set output directory to ${BINARY_DIR}/Unittests
    set_target_properties(unittests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Unittests)
    # Set compiler flags
    set_target_properties(unittests PROPERTIES COMPILE_FLAGS "-g -pedantic -ansi -Wno-long-long")
    acg_copy_after_build(unittests ${CMAKE_CURRENT_SOURCE_DIR}/TestFiles ${CMAKE_BINARY_DIR}/Unittests/)

else(GTEST_FOUND)
    message("Google testing framework was not found!")
endif(GTEST_FOUND)
