#pragma once

#include <string>

class RenderStage
{
private:
	std::string windowTitle;
public:
	RenderStage(int width, int height, std::string windowTitle);
	~RenderStage();
	void Render();
};