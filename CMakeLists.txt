cmake_minimum_required(VERSION 2.8.12...3.13)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-Werror")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set(CMAKE_INCLUDES_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/includes )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set(CMAKE_DOCUMENTATION_DIRECTORY ${CMAKE_BINARY_DIR}/docs )
# Install Dirs
set(INSTALL_LIB_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib)
set(INSTALL_INCLUDE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include)
set(INSTALL_PKGCONFIG_DIRECTORY ${INSTALL_LIB_DIRECTORY}/pkgconfig)

set(CMAKE_VERBOSE_MAKEFILE ON)
make_directory(${CMAKE_DOCUMENTATION_DIRECTORY})
set(EP_DEPENDENCIES_PATH "${CMAKE_BINARY_DIR}/Dependencies")

# Project Properties
set(PROJECT_NAME         "ArmTTS")
set(PROJECT_VERSION      "1.0.0")
set(PROJECT_DESCRIPTION  "ML based Text-to-Speech(TTS) engine for the Armenian language")
set(PROJECT_AUTHOR_NAME  "Albert Grigoryan")
set(PROJECT_AUTHOR_EMAIL "albert_grigoryan@yahoo.com")
project(${PROJECT_NAME})

# Add general dependencies
add_subdirectory(third_party/boost)
add_subdirectory(third_party/fftw)
add_subdirectory(third_party/log4cpp)
add_subdirectory(third_party/nlohmannjson)
add_subdirectory(third_party/sndfile)
add_subdirectory(third_party/tensorflow_lite)

# Add projects.
add_subdirectory(src/logger)
add_subdirectory(src/utilities)
add_subdirectory(src/phonetic_engine)
add_subdirectory(src/inference_engine)
add_subdirectory(src/armtts)

# Add the tests and the documentation.
add_subdirectory(third_party/gtest)
add_subdirectory(third_party/doxygen)
add_subdirectory(tests/unit)
add_subdirectory(src/doxygen)