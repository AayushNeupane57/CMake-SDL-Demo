#pragma once
#include <stdlib.h>
#include <vector>
#include <memory>

#include "app.h"
#include "scene.h"
#include "demo_scene.h"

class DemoApp: public App {
public:
	void Initialize();
	void Update();
	void Render();
	void Destroy();
private:
	std::vector<std::shared_ptr<Scene>> scenes;
};
