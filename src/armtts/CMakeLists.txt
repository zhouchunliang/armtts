get_filename_component(SUB_PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file(GLOB SOURCES "src/*.c" "src/*.cpp")
add_library(${SUB_PROJECT_NAME} SHARED ${SOURCES})
add_dependencies(${SUB_PROJECT_NAME}
        inference_engine
        logger
        phonetic_engine
        boost
)

target_link_directories(${SUB_PROJECT_NAME} PUBLIC
        ${BOOST_LIB_DIR}
)
target_link_libraries(${SUB_PROJECT_NAME} LINK_PUBLIC
        logger
        inference_engine
        phonetic_engine
        ${BOOST_LIBRARIES}
)
target_include_directories(${SUB_PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        ${BOOST_INCLUDE_DIR}
)

# Install
install(TARGETS ${SUB_PROJECT_NAME} DESTINATION ${INSTALL_LIB_DIRECTORY})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${INSTALL_INCLUDE_DIRECTORY}
        FILES_MATCHING PATTERN "*.hpp"
)
install(FILES armtts.pc
        DESTINATION ${INSTALL_PKGCONFIG_DIRECTORY}
)