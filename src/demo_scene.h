#pragma once
#include <iostream>
#include <memory>
#include <stdio.h>
#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl_gl3.h>

#include "scene.h"
#include "stage_image.h"
#include "input.h"
#include "demo_scene.h"
#include "app_platform.h"
#include "gl_utils.h"
#include "renderer.h"
#include "file_dialog.h"

class DemoScene : public Scene {
	void Initialize();	
	void Render();
	void Update();
private:
	GLuint vertexArrayID, programID, matrixID;
  std::unique_ptr<StageImage> image;
  glm::vec3 cameraPosition;
  bool show_test_window;
  std::string showHideButtonText;
};
