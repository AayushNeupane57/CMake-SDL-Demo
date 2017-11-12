#pragma once

#include <string>
#include <vector>
#ifndef __APPLE__
	#include <GL/glew.h>  // Don't need GLEW ON MacOS
	#include <SDL_opengl.h>
#else                   // Include standard MacOS OpenGL headers
	#include <OpenGL/gl3.h>
#endif
#include <glm/glm.hpp>

class StageImage {
private:
	unsigned char *data;
	int width, height, components;
	std::vector<GLfloat> _vertexBuffer;
	std::vector<GLfloat> _buildVertexBuffer(int img_w, int img_h);
public:
  glm::vec3 Position, Origin, Scale;
	float CameraZoom = 0.2f;
	GLuint textureID;
	StageImage();
	~StageImage();
	void SetImage(std::string filepath);
	std::vector<GLfloat> GetVertexBuffer();
  glm::mat4 GetMatrix(const glm::vec3 &cameraPosition, const glm::vec2 &windowSize);
};
