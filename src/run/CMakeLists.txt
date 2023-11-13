# CMakeLists.txt - CMake configuration file for EPANET MSX runner
#
# Created: Nov 13, 2023
# Updated: Nov 13, 2023
#
# Author: Caleb Buahin
#         US EPA ORD/CESER
#

if(APPLE)
    set(RPATH_ROOT "@loader_path")
else()
    set(RPATH_ROOT "$ORIGIN")
endif()

set(PACKAGE_RPATH "${RPATH_ROOT}/../lib;${RPATH_ROOT}/../extern")


# Creates the EPANET command line executable
add_executable(runepanetmsx
    msxmain.c
)

target_include_directories(runepanetmsx
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(runepanetmsx
    LINK_PUBLIC
        epanet2
        epanetmsx
)

# Set up rpath for runswmm inside install package
set_target_properties(runepanetmsx
    PROPERTIES
        MACOSX_RPATH TRUE
        SKIP_BUILD_RPATH FALSE
        BUILD_WITH_INSTALL_RPATH FALSE
        INSTALL_RPATH "${PACKAGE_RPATH}"
        INSTALL_RPATH_USE_LINK_PATH TRUE
)

install(TARGETS runepanetmsx
    DESTINATION "${TOOL_DIST}"
)
