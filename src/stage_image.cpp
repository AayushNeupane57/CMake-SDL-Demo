#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
#include "stage_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

StageImage::StageImage() { }
StageImage::~StageImage() { }

void StageImage::SetImage(std::string imagepath)
{
	this->data = stbi_load(
                         imagepath.c_str(),
                         &(this->width),
                         &(this->height),
                         &(this->components),
                         4);
	_vertexBuffer = _buildVertexBuffer(this->width, this->height);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->data);
	stbi_image_free(data);
}

std::vector<GLfloat> StageImage::GetVertexBuffer()
{
	return this->_vertexBuffer;
}

std::vector<GLfloat> StageImage::_buildVertexBuffer(int img_w, int img_h) 
{

	//  p5 = p2
	//  p4 = p3

	//  p1--p5
	//  |  / |
	//  | /  |
	//  p3--p6

	float wf = (float)img_w;
	float wh = (float)img_h;
	return {
		0.0f, 0.0f, 0.0f,	0.0f, 1.0f,		// p1
		wf, 0.0f, 0.0f, 1.0f, 1.0f,			// p2
		0.0f,  wh, 0.0f, 0.0f, 0.0f,		// p3
		0.0f,  wh, 0.0f, 0.0f, 0.0f,		// p4 = p3
		wf, 0.0f, 0.0f,	1.0f, 1.0f,			// p5 = p2
		wf, wh, 0.0f,  1.0f, 0.0f				// p6
	};
}

glm::mat4 StageImage::GetMatrix(const glm::vec3 &cameraPosition, const glm::vec2 &windowSize)
{
  glm::mat4 img_tMatrix = glm::translate(
                                         glm::mat4(1.0f),
                                         glm::vec3(-this->width / 2, -this->height / 2, 0));
  glm::mat4 t_matrix = glm::translate(glm::mat4(1.0f), cameraPosition);

	float l = (windowSize.x - (windowSize.x * (1 / this->CameraZoom))) / 2;
	float r = l + (windowSize.x * (1 / this->CameraZoom));
	float t = (windowSize.y - (windowSize.y * (1 / this->CameraZoom))) / 2;
	float b = t + (windowSize.y * (1 / this->CameraZoom));
  
	// order of t and b below reversed from normal (l, r, b, t) to flip image
	glm::mat4 proj_matrix = glm::ortho(l, r, t, b, 0.0f, 1.0f);
  glm::mat4 srt_matrix = proj_matrix * t_matrix * img_tMatrix;
  
  return srt_matrix;
}
