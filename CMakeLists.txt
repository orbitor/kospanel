#
#   kospanel: The Kind Of Small Panel
#
#   Copyright (c) 2013 James DeLisle
#   See the `LICENSE` file for complete details.
#

cmake_minimum_required(VERSION 2.8)

find_package(X11 REQUIRED)
find_package(Freetype REQUIRED)

set (KOSPANEL_ROOT
        ${CMAKE_CURRENT_LIST_DIR}
        CACHE PATH
        "Kospanel root directory"
)

set (KOSPANEL_SOURCES
        ${KOSPANEL_ROOT}/kospanel.c
)

include_directories(
        ${KOSPANEL_ROOT}
        ${FREETYPE_INCLUDE_DIRS}
)

add_executable(kospanel
        ${KOSPANEL_SOURCES}
)

target_link_libraries(kospanel
        X11
        Xext
        Xft
        Xrender
        Xau
        Xdmcp
        Xinerama
        xcb
        freetype
        fontconfig
)
        
