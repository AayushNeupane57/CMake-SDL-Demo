#pragma once
#include <iostream>
#include <memory>
#include "scene.h"
#include "stage_image.h"

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
