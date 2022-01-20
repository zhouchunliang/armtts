include(ExternalProject)

set(NAME boost)
set(VERSION "1.75.0")
set(PREFIX ${EP_DEPENDENCIES_PATH}/${NAME})
set(BOOST_BOOTSTRAP_COMMAND "./bootstrap.sh")
set(BOOST_B2_COMMAND "./b2")

message (STATUS "Adding ${NAME}:${VERSION} as an external project.")

ExternalProject_Add(${NAME}
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  UPDATE_DISCONNECTED ON
  TAG boost-${VERSION}
  BUILD_IN_SOURCE ON
  UPDATE_COMMAND ""
  PATCH_COMMAND git submodule update --init
  CONFIGURE_COMMAND ${BOOST_BOOTSTRAP_COMMAND}
  BUILD_COMMAND ${BOOST_B2_COMMAND} install
    --without-python
    --without-mpi
    --disable-icu
    --prefix=${PREFIX}
    --threading=single,multi
    --link=shared
    --variant=release
    -j8
  INSTALL_COMMAND ""
  INSTALL_DIR ${PREFIX}
)

set(BOOST_LIB_DIR ${PREFIX}/lib/ PARENT_SCOPE)
set(BOOST_INCLUDE_DIR ${PREFIX}/include/ PARENT_SCOPE)
set(BOOST_LIBRARIES
        -lboost_filesystem
PARENT_SCOPE)
