#
# CMakeLists.txt - CMake configuration file for s
# Created: 11-13-2023
# Updated: 11-13-2023
#
# Author: Caleb Buahin
#         US EPA ORD/CESER
#

# list(APPEND CMAKE_LIBRARY_PATH "usr/local/opt")
# list(APPEND CMAKE_LIBRARY_PATH "usr/local/opt/libomp")
# list(APPEND CMAKE_LIBRARY_PATH "usr/local/opt/libomp/lib")
find_package(OpenMP 
    HINTS 
        "usr/local/opt" 
        "usr/local/opt/libomp" 
        "usr/local/opt/libomp/lib"
        "opt/homebrew/opt"
        "opt/homebrew/opt/libomp"
        "opt/homebrew/opt/libomp/lib"
)


set(EPANETMSX_PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/epanetmsx.h
)

file(GLOB
    EPANETMSX_SOURCES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c *.h *.dat
)

if(BUILD_DEF)
    # Builds library with def file interface for backward compatibility
    set_source_files_properties(${PROJECT_SOURCE_DIR}/include/epanetmsx.def
        PROPERTIES_HEADER_FILE_ONLY TRUE
    )

    add_library(epanetmsx
            ${EPANETMSX_SOURCES}
            ${PROJECT_SOURCE_DIR}/include/epanetmsx.def
    )

else()
    # Performs standard library build
    add_library(epanetmsx
            ${EPANETMSX_SOURCES}
    )

endif()

target_link_libraries(epanetmsx
    PUBLIC
        epanet2
)

# if openmp is availabe set USE_OPENMP define for target
if(OpenMP_C_FOUND)
    target_compile_definitions(epanetmsx
        PUBLIC
            USE_OPENMP
    )
endif()

# Sets MSVC compiler flags
target_compile_options(epanetmsx
    PUBLIC
        "$<$<C_COMPILER_ID:MSVC>:"
            "$<$<CONFIG:Release>:/GL>"
            "$<$<CONFIG:Release>:/fp:fast>"
            "$<$<CONFIG:Release>:/Zi>"
        ">"
        $<$<C_COMPILER_ID:AppleClang>:
            $<$<STREQUAL:"${CMAKE_GENERATOR}","Ninja">:-O3>
        >
)

target_link_options(epanetmsx
    PUBLIC
        "$<$<C_COMPILER_ID:MSVC>:"
            "$<$<CONFIG:Release>:/LTCG:incremental>"
        ">"
)

include(GenerateExportHeader)
generate_export_header(epanetmsx 
    BASE_NAME epanetmsx
    EXPORT_MACRO_NAME MSXDLLEXPORT
    EXPORT_FILE_NAME epanetmsx_export.h
    STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC
)

file(COPY ${CMAKE_CURRENT_BINARY_DIR}/epanetmsx_export.h
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include
)



target_link_libraries(epanetmsx
    PUBLIC
        $<$<NOT:$<BOOL:$<C_COMPILER_ID:MSVC>>>:m>
        $<$<BOOL:${OpenMP_C_FOUND}>:OpenMP::OpenMP_C>
        $<$<BOOL:${OpenMP_AVAILABLE}>:omp>
)


target_include_directories(epanetmsx
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/EPANET2.2/SRC_engines/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${INCLUDE_DIST}>

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)


install(TARGETS epanetmsx EXPORT epanetmsxTargets
    RUNTIME    DESTINATION "${TOOL_DIST}"
    LIBRARY    DESTINATION "${LIBRARY_DIST}"
    ARCHIVE    DESTINATION "${LIBRARY_DIST}"
    FRAMEWORK  DESTINATION "${TOOL_DIST}"
)

# Create target import scripts so other cmake projects can use swmm libraries
install(
    EXPORT
        epanetmsxTargets
    DESTINATION
        "${CONFIG_DIST}"
    FILE
        epanetmsx-config.cmake
)

install(
    FILES 
        ${EPANETMSX_PUBLIC_HEADERS} 
    DESTINATION 
        "${INCLUDE_DIST}"
)
