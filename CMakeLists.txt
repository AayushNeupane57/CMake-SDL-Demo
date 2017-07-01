cmake_minimum_required (VERSION 2.6)
project (cmake_sdl)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

set (SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_executable(cmake_sdl ${SOURCE_FILES})
target_link_libraries(cmake_sdl ${SDL2_LIBRARY})