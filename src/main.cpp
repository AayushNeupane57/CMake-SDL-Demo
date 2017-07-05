#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
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

	// init glew
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
			cout << "Error initializing GLEW: " << glewGetErrorString(glewError) << endl;
	}

	// teardown
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
	return 0;
}
