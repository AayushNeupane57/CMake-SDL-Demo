#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
#include "stage_image.h"

#include <vector>

StageImage::StageImage() { }
StageImage::~StageImage() {
	stbi_image_free(this->data);
}

void StageImage::SetImage(std::string imagepath)
{
	this->data = stbi_load(imagepath.c_str(), &(this->width), &(this->height), &(this->components), 0);
	_vertexBuffer = _buildVertexBuffer(this->width, this->height);
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
		0.0f, 0.0f, 0.0f,		// p1
		wf, 0.0f, 0.0f,			// p2
		0.0f,  wh, 0.0f,		// p3
		0.0f,  wh, 0.0f,		// p4 = p3
		wf, 0.0f, 0.0f,			// p5 = p2
		wf, wh, 0.0f				// p6
	};
}