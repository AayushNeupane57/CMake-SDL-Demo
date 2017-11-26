#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#ifndef __APPLE__
	#include <GL/glew.h>  // Don't need GLEW ON MacOS
	#include <SDL_opengl.h>
#else                   // Include standard MacOS OpenGL headers
	#include <OpenGL/gl3.h>
#endif

namespace GLUtils {
	GLuint CreateProgram(std::string vertex_shader_file_path, std::string fragment_shader_file_path);
};
