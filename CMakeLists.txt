cmake_minimum_required (VERSION 2.6)
project (cmake_sdl)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

set (SOURCE_FILES 
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/imconfig.h
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_internal.h
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui.h
    ${CMAKE_SOURCE_DIR}/libs/imgui/stb_rect_pack.h
    ${CMAKE_SOURCE_DIR}/libs/imgui/stb_textedit.h
    ${CMAKE_SOURCE_DIR}/libs/imgui/stb_truetype.h)

add_executable(cmake_sdl ${SOURCE_FILES})
target_link_libraries(cmake_sdl ${SDL2_LIBRARY})