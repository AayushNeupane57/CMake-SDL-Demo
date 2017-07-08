#include <SDL.h>
#ifndef __APPLE__
	#include <GL/glew.h>  // Don't need GLEW ON MacOS
#else                   // Include standard MacOS OpenGL headers
	#include <OpenGL/glu.h>
	#include <OpenGL/glext.h>
#endif
#include <SDL_opengl.h>
#include <iostream>

using namespace std;
int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		cout << "SDL failed to initialize: " << SDL_GetError() << endl;
		return 1;
  }
	cout << "SDL initialized successfully" << endl;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_Window* mainWindow = SDL_CreateWindow(
			"SDL2 OpenGL Demo",				// title
			SDL_WINDOWPOS_CENTERED,		// pos x
			SDL_WINDOWPOS_CENTERED,		// pos y
			1280,											// width
			720,											// height
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	SDL_GLContext glContext = SDL_GL_CreateContext(mainWindow);
  if (glContext == NULL)
  {
    cout << "OpenGL context failed to initialize" << endl;
  }
	// init glew, if platform requires it
#ifndef __APPLE__
  GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
			cout << "Error initializing GLEW: " << glewGetErrorString(glewError) << endl;
	}
#endif
  while (true)
  {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			// process pending window events (move, resize, system ui rendering, etc.)
		}
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(mainWindow);
    SDL_Delay(16);
  }

	// teardown
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
	return 0;
}
