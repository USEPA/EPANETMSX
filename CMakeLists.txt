# CMakeLists.txt - CMake configuration file for EPANETMSX 1.1

cmake_minimum_required(VERSION 3.9)

project(EPANETMSX LANGUAGES C)

# Append local dir to module search path
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
# Add building the command line executable from the run directory
add_subdirectory(run)

# Sets the output directory for executables and libraries.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Sets the position independent code property for all targets
SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

IF (APPLE)
  set(INSTALL_NAME_DIR @executable_path/../lib)
  set(CMAKE_MACOSX_RPATH 1)
ENDIF (APPLE)

IF (MSVC)
  set(CMAKE_C_FLAGS_RELEASE "/GL")
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE -MT)
ENDIF (MSVC)

# configure file groups
file(GLOB MSX_SOURCES RELATIVE ${PROJECT_SOURCE_DIR} src/*.c)

# the shared library
find_package(OpenMP REQUIRED)
IF(MSVC AND "${CMAKE_VS_PLATFORM_NAME}" MATCHES "(Win32)")
  message("   ************************************")
  message("   Configuring with msx.def mapping")
  message("   ************************************")
  find_library(EPANET_LIB epanet2 NAMES epanet2 PATHS ${PROJECT_SOURCE_DIR}/include/x86 NO_DEFAULT_PATH)
  add_library(epanetmsx SHARED ${MSX_SOURCES} ${PROJECT_SOURCE_DIR}/include/epanetmsx.def)
  set_source_files_properties(${PROJECT_SOURCE_DIR}/include/msx.def PROPERTIES_HEADER_FILE_ONLY TRUE)
  target_link_libraries(epanetmsx ${EPANET_LIB} OpenMP::OpenMP_C)
ELSE(TRUE)
  find_library(EPANET_LIB epanet2 NAMES epanet2 PATHS ${PROJECT_SOURCE_DIR}/include/x64 NO_DEFAULT_PATH)
  add_library(epanetmsx SHARED ${MSX_SOURCES})
  target_link_libraries(epanetmsx ${EPANET_LIB} OpenMP::OpenMP_C)
ENDIF(MSVC AND "${CMAKE_VS_PLATFORM_NAME}" MATCHES "(Win32)")

target_include_directories(epanetmsx PUBLIC ${PROJECT_SOURCE_DIR}/include)

