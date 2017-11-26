cmake_minimum_required (VERSION 3.1)
project (cmake_sdl)

set(CMAKE_CXX_STANDARD 14)

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
set(INCLUDE_DIRECTORIES
  ${SDL2_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/libs/glm
  ${CMAKE_SOURCE_DIR}/libs/imgui)
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
    ${CMAKE_SOURCE_DIR}/libs/stb_image.h
    ${CMAKE_SOURCE_DIR}/src/render_stage.h
    ${CMAKE_SOURCE_DIR}/src/render_stage.cpp
    ${CMAKE_SOURCE_DIR}/src/stage_image.h
    ${CMAKE_SOURCE_DIR}/src/stage_image.cpp
    ${CMAKE_SOURCE_DIR}/src/gl_utils.h
    ${CMAKE_SOURCE_DIR}/src/gl_utils.cpp)

IF(APPLE)
    list(APPEND SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/file_dialog_mac.h)
    list(APPEND SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/file_dialog_mac.mm)
ENDIF()

add_executable(cmake_sdl ${SOURCE_FILES})
target_link_libraries(cmake_sdl ${LINK_LIBRARIES})

set(ASSET_COPY_DIR $<TARGET_FILE_DIR:cmake_sdl>/assets)

IF(NOT(APPLE))
		set(ASSET_COPY_DIR $<TARGET_FILE_DIR:cmake_sdl>/../assets)
		set(GLEW_BIN $ENV{GLEW}/bin/Release/Win32/glew32.dll)

		# Copy glew32.dll to output directory
		add_custom_command(
				TARGET cmake_sdl POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy ${GLEW_BIN} $<TARGET_FILE_DIR:cmake_sdl>
		)
ENDIF()

add_custom_command(
    TARGET cmake_sdl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${ASSET_COPY_DIR}
)
