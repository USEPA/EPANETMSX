#
# CMakeLists.txt - CMake configuration file for s
# Created: 11-13-2023
# Updated: 11-13-2023
#
# Author: Caleb Buahin
#         US EPA ORD/CESER
#

find_package(OpenMP)


set(EPANET_PUBLIC_HEADERS
    ${PROJECT_SOURCE_DIR}/include/epanetmsx.h
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

if(UNIX)
    target_link_libraries(epanetmsx
        PRIVATE
            m
    )
endif()

target_include_directories(epanetmsx
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/EPANET2.2/SRC_engines/include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${INCLUDE_DIST}>
)

# include(GenerateExportHeader)
# generate_export_header(epanet2
#     STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC
# )

# if(APPLE) 
#     set(LIB_ROOT "@loader_path")
# else()
#     set(LIB_ROOT "$ORIGIN")
# endif()

# set_target_properties(epanet2
#     PROPERTIES
#         MACOSX_RPATH TRUE
#         SKIP_BUILD_RPATH FALSE
#         BUILD_WITH_INSTALL_RPATH FALSE
#         INSTALL_RPATH "${LIB_ROOT};${PACKAGE_RPATH};"
#         INSTALL_RPATH_USE_LINK_PATH TRUE
# )


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
