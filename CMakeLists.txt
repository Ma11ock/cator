cmake_minimum_required(VERSION 3.20)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE release)
  message("Setting build type to release.")
else()
  set(CMAKE_BUILD_TYPE debug)
  set(CHAR_TESTS 1)
  message("Build type is set to debug.")
endif()

project(cator VERSION 0.1.0)

include(ProcessorCount)
ProcessorCount(NCPUS)

set(OpenGL_GL_PREFERENCE GLVND)

include(CheckSymbolExists)
include(CheckIncludeFile)
include(CMakeDetermineSystem)
include(ExternalProject)
include(GNUInstallDirs)

add_subdirectory(src)



