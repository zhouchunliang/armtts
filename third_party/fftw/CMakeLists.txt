include(ExternalProject)

set(NAME fftw)
set(VERSION 3.3.10)
set(PREFIX ${EP_DEPENDENCIES_PATH}/${NAME})

message (STATUS "Adding ${NAME}:${VERSION} as an external project.")

ExternalProject_Add(${NAME}
  URL http://fftw.org/fftw-${VERSION}.tar.gz
  UPDATE_DISCONNECTED ON
  CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX=${PREFIX}"
    "-DCMAKE_BUILD_TYPE:STRING=Release"
    "-DENABLE_FLOAT=ON"
)

set(FFTW_INCLUDE_DIR ${PREFIX}/include PARENT_SCOPE)
set(FFTW_LIB_DIR ${PREFIX}/lib PARENT_SCOPE)
set(FFTW_LIBRARIES
    -lfftw3f
PARENT_SCOPE)