cmake_minimum_required (VERSION 2.6)
project (cmake_sdl)

# Output Directory:
# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR/bin})

# set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")


# Manually setting paths for now, works on Windows like this:
set (SDL2_INCLUDE ${CMAKE_SOURCE_DIR}/include/SDL2/include)
# Link against SDL2main.lib on Windows:
set (SDL2_LIBRARY ${CMAKE_SOURCE_DIR}/include/SDL2/lib/x86/SDL2main.lib)

include_directories(${SDL2_INCLUDE})

set (SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_executable(cmake_sdl ${SOURCE_FILES})
target_link_libraries(cmake_sdl ${SDL2_LIBRARY})