#ifndef __APPLE__
	#include <GL/glew.h>  // Don't need GLEW ON MacOS
	#include <SDL_opengl.h>
#else                   // Include standard MacOS OpenGL headers
	#include <OpenGL/gl3.h>
#endif

#pragma once
class Renderer {
public:
	static void DrawVertices(GLuint vaoId, GLenum mode, int vertexCount);
};
