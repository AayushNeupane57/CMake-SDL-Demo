#include <string>
#include <iostream>
#include "render_stage.h"

RenderStage::RenderStage(int width, int height, std::string windowTitle)
{
	this->windowTitle = windowTitle;
	std::cout << "Creating window " << this->windowTitle << std::endl;
}

RenderStage::~RenderStage()
{
	std::cout << "Destroying window " << this->windowTitle << std::endl;
}

void RenderStage::Render()
{
	std::cout << "Rendering to window " << this->windowTitle << std::endl;
}