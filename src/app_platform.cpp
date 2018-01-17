#include "app_platform.h"

SDL_Window* GetWindowPtr() {
  return windowPtr;
}

Input* InputRef() {
	return input;
}

void CreateWindow() {
  windowPtr = SDL_CreateWindow(
    "SDL2 OpenGL Demo",                // title
    SDL_WINDOWPOS_CENTERED,            // pos x
    SDL_WINDOWPOS_CENTERED,            // pos y
    1280,                              // width
    720,                               // height
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
  );
}

int RunApp(std::shared_ptr<App> app) {
	// Initialize SDL:
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL failed to initialize video: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	CreateWindow();
	
	SDL_GLContext glContext = SDL_GL_CreateContext(windowPtr);
	if (glContext == NULL) {
		std::cout << "OpenGL context failed to initialize" << std::endl;
		return 1;
	}
#ifndef __APPLE__
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		std::cout << "Error initializing GLEW: " << glewGetErrorString(glewError) << std::endl;
		return 1;
	}
#endif // !__APPLE__

	// Run the app specific initialization logic:
  app->Initialize();

	// Start the main loop, handle windowing events (TODO):
	input = new Input();
	mainLoop = new main_loop();
	mainLoop->run(
		[app] { app->Render(); },
		[app] {
			ProcessInputEvents(input);
			app->Update();
		}
	);
	// Teardown:
	app->Destroy();
	SDL_GL_DeleteContext(glContext);
	SDL_Quit();
	return 0;
}

void ProcessInputEvents(Input* in) {
	input->FlushEvents();
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			input->FlushEvents();
			mainLoop->stop();
		}
		else {
			input->PushInputEvent(event);
		}
	}
}