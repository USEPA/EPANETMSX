#
# CMakeLists.txt - CMake configuration file for EPANETMSX 2.0
#
# Created: Nov 13, 2023
# Modified: Nov 13, 2023
#
# Author: Michael E. Tryby
#         US EPA ORD/CESER
#


cmake_minimum_required(VERSION 3.9)

if("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    message(FATAL_ERROR "In-source builds are disabled.")
endif()

project(
  "EPANETMSX"
  VERSION 2.0.0
  DESCRIPTION "Multi-species extension for EPANET"
  LANGUAGES C CXX
)

# Append local dir to module search path
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Sets default install prefix when cmakecache is initialized for first time
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "..." FORCE)
endif()


# Sets the output directory for executables and libraries.
set(TOOL_DIST "bin")
set(INCLUDE_DIST "include")
set(LIBRARY_DIST "lib")
set(CONFIG_DIST  "cmake")

# Sets the position independent code property for all targets
SET(CMAKE_POSITION_INDEPENDENT_CODE ON)


# option(BUILD_TESTS "Build component tests (requires Boost)" OFF)
option(BUILD_DEF   "Builds library with def file interface" OFF)

# Added option to statically link libraries to address GitHub
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_subdirectory(EPANET2.2/SRC_engines)

add_subdirectory(src/solver)

add_subdirectory(src/run)

set(CMAKE_INSTALL_OPENMP_LIBRARIES TRUE)
include(InstallRequiredSystemLibraries)

# Configure CPack driven installer package
set(CPACK_GENERATOR "ZIP;TGZ")
set(CPACK_PACKAGE_VENDOR "US_EPA")
set(CPACK_ARCHIVE_FILE_NAME "epanet-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

include(CPack)