# EPANETMSX COMMAND LINE EXECUTABLE
cmake_minimum_required (VERSION 2.8.8)

# Sets for output directory for executables and libraries.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(RELEASEVAR, "Release")
set(DEBUGVAR, "Debug")

# Sets the position independent code property for all targets.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# Link to multi-threaded static runtime library
IF (MSVC)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE -MT)
ENDIF (MSVC)


# Set up file groups for exe target
set(MSX_CLI_SOURCES msxmain.c)
include_directories(include) 

source_group("CLI" FILES ${MSX_CLI_SOURCES})

# Creates the EPANET-MSX command line executable
add_executable(runepanetmsx ${MSX_CLI_SOURCES})
target_compile_definitions(runepanetmsx PUBLIC FOO=1)

IF(MSVC AND "${CMAKE_VS_PLATFORM_NAME}" MATCHES "(Win32)")
  find_library(EPANET_LIB epanet2 NAMES epanet2 PATHS ${PROJECT_SOURCE_DIR}/include/x86 NO_DEFAULT_PATH)
  target_link_libraries(runepanetmsx LINK_PUBLIC epanetmsx ${EPANET_LIB})
  add_custom_command(
        TARGET runepanetmsx 
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/include/x86/epanet2.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/epanet2.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/include/runvc.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/runvc.bat)
else(TRUE)
  find_library(EPANET_LIB epanet2 NAMES epanet2 PATHS ${PROJECT_SOURCE_DIR}/include/x64 NO_DEFAULT_PATH)
  target_link_libraries(runepanetmsx LINK_PUBLIC epanetmsx ${EPANET_LIB})
  add_custom_command(
        TARGET runepanetmsx
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/include/x64/epanet2.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/epanet2.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/include/runvc.bat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/runvc.bat)
endif(MSVC AND "${CMAKE_VS_PLATFORM_NAME}" MATCHES "(Win32)")
