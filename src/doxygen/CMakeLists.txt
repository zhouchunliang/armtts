set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
configure_file(${doxyfile_in} ${doxyfile} @ONLY)

add_custom_target(docs ALL
  ${DOXYGEN_EXECUTABLE} ${doxyfile}
  WORKING_DIRECTORY ${CMAKE_DOCUMENTATION_DIRECTORY}
  COMMENT "Generating API documentation with Doxygen"
  VERBATIM
)

add_dependencies(docs doxygen)