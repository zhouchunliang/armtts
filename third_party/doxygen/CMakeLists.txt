include(ExternalProject)

set(NAME doxygen)
set(VERSION 1_8_17)
set(PREFIX ${EP_DEPENDENCIES_PATH}/${NAME})

message (STATUS "Adding ${NAME}:${VERSION} as an external project.")

ExternalProject_Add(${NAME}
  URL https://github.com/doxygen/doxygen/archive/Release_${VERSION}.tar.gz
  UPDATE_DISCONNECTED ON
  CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX=${PREFIX}"
    "-DCMAKE_BUILD_TYPE:STRING=Release"
)

set(DOXYGEN_EXECUTABLE ${PREFIX}/bin/doxygen PARENT_SCOPE)
