#
#   kospanel: The Kind Of Small Panel
#
#   Copyright (c) 2013 James DeLisle
#   See the `LICENSE` file for complete details.
#

cmake_minimum_required(VERSION 2.8)

find_package(X11 REQUIRED)
#find_package(Freetype REQUIRED)

set (KOSPANEL_ROOT
        ${CMAKE_CURRENT_LIST_DIR}
        CACHE PATH
        "Kospanel root directory"
)

set (KOSPANEL_SOURCES
        ${KOSPANEL_ROOT}/kosp_x11.c
        ${KOSPANEL_ROOT}/kosp_base.c
        ${KOSPANEL_ROOT}/kosp_list.c
        ${KOSPANEL_ROOT}/kosp_ui.c
        ${KOSPANEL_ROOT}/kosp_app.c
        ${KOSPANEL_ROOT}/main.c
)

include_directories(
        ${KOSPANEL_ROOT}
        ${X11_X11_INCLUDE_PATH}
        ${FREETYPE_INCLUDE_DIRS}
)

set_source_files_properties(
        ${KOSPANEL_SOURCES}
        PROPERTIES
        COMPILE_FLAGS
        "-std=c99 -Wall"
)

add_executable(kospanel
        ${KOSPANEL_SOURCES}
)

target_link_libraries(kospanel
        ${X11_X11_LIB}
        ${X11_Xext_LIB}
        ${X11_Xft_LIB}
        ${X11_Xrender_LIB}
        ${X11_Xau_LIB}
        ${X11_Xdmcp_LIB}
        ${X11_Xinerama_LIB}
        ${X11_xcb_LIB}
        ${FREETYPE_LIBRARY}
)
        
