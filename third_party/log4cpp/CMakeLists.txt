include(ExternalProject)

set(NAME log4cpp)
set(VERSION 2.9.1)
set(PREFIX ${EP_DEPENDENCIES_PATH}/${NAME})

message (STATUS "Adding ${NAME}:${VERSION} as an external project.")

ExternalProject_Add(${NAME}
  GIT_REPOSITORY https://github.com/orocos-toolchain/log4cpp.git
  UPDATE_DISCONNECTED ON
  CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX=${PREFIX}"
    "-DCMAKE_BUILD_TYPE:STRING=Release"
)

set(LOG4CPP_INCLUDE_DIR ${PREFIX}/include/orocos PARENT_SCOPE)
set(LOG4CPP_LIB_DIR ${PREFIX}/lib PARENT_SCOPE)
set(LOG4CPP_LIBRARIES orocos-log4cpp PARENT_SCOPE)