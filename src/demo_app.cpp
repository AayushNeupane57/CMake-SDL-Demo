#include "demo_app.h"

void DemoApp::Initialize() {
	scenes = {
		std::make_shared<DemoScene>()
	};
	scenes[0]->Initialize();
}

void DemoApp::Update() {
	scenes[0]->Update();
}

void DemoApp::Render() {
	scenes[0]->Render();
}

void DemoApp::Destroy() {}