get_filename_component(SUB_PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file(GLOB SOURCES "src/*.c" "src/*.cpp")
add_library(${SUB_PROJECT_NAME} SHARED ${SOURCES})
add_dependencies(logger
        log4cpp
)

target_link_directories(${SUB_PROJECT_NAME} PUBLIC
        ${LOG4CPP_LIB_DIR}
)
target_link_libraries(${SUB_PROJECT_NAME} LINK_PUBLIC
        ${LOG4CPP_LIBRARIES}
)
target_include_directories(${SUB_PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        ${LOG4CPP_INCLUDE_DIR}
)
# Install
install(TARGETS ${SUB_PROJECT_NAME} DESTINATION ${INSTALL_LIB_DIRECTORY})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${INSTALL_INCLUDE_DIRECTORY}
        FILES_MATCHING PATTERN "*.hpp"
)