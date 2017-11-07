#pragma once

#include <string>
#include <vector>
#ifndef __APPLE__
	#include <GL/glew.h>  // Don't need GLEW ON MacOS
	#include <SDL_opengl.h>
#else                   // Include standard MacOS OpenGL headers
	#include <OpenGL/gl3.h>
#endif

class StageImage {
private:
	unsigned char *data;
	int width, height, components;
	std::vector<GLfloat> _vertexBuffer;
	std::vector<GLfloat> _buildVertexBuffer(int img_w, int img_h);
public:
	StageImage();
	~StageImage();
	void SetImage(std::string filepath);
	std::vector<GLfloat> GetVertexBuffer();
};