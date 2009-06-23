include (ACGDoxygen)

if (TARGET doc)
  acg_copy_after_build (doc "${CMAKE_CURRENT_SOURCE_DIR}/html" "${CMAKE_BINARY_DIR}/Build/${ACG_PROJECT_DATADIR}/Doc/html")
endif ()
