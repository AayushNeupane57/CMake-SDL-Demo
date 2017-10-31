cmake_minimum_required (VERSION 2.6)
project (cmake_sdl)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")

find_package(SDL2 REQUIRED)

# look for glew relative to the "GLEW" environment variable path
# Need to set GLEW_INCLUDE_DIR and GLEW_LIBRARY_RELEASE
IF(NOT(APPLE))
  set(GLEW_INCLUDE_DIR $ENV{GLEW}/include)
  set(GLEW_LIBRARY_RELEASE $ENV{GLEW}/lib/Release/Win32/glew32.lib)
  find_package(GLEW REQUIRED)
ENDIF()

find_package(OPENGL REQUIRED)
set(INCLUDE_DIRECTORIES ${SDL2_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS})
set(LINK_LIBRARIES ${OPENGL_LIBRARIES} ${SDL2_LIBRARY})

IF(NOT(APPLE))
	list(APPEND INCLUDE_DIRECTORIES ${GLEW_INCLUDE_DIRS})
  list(APPEND LINK_LIBRARIES ${GLEW_LIBRARIES})
ENDIF()

include_directories(${INCLUDE_DIRECTORIES})

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
    ${CMAKE_SOURCE_DIR}/libs/imgui/stb_truetype.h
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_impl_sdl_gl3.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_impl_sdl_gl3.h
    ${CMAKE_SOURCE_DIR}/libs/stb_image.h)

add_executable(cmake_sdl ${SOURCE_FILES})
target_link_libraries(cmake_sdl ${LINK_LIBRARIES})

add_custom_command(
    TARGET cmake_sdl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:cmake_sdl>/assets
)
