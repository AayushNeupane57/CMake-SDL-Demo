#pragma once
#include <SDL.h>
#include <memory>
#ifndef __APPLE__
	#include <GL/glew.h>  // Don't need GLEW ON MacOS
	#include <SDL_opengl.h>
#else                   // Include standard MacOS OpenGL headers
	#include <OpenGL/gl3.h>
#endif

#include "app.h"
#include "main_loop.h"

static SDL_Window* windowPtr;

void CreateWindow();
SDL_Window* GetWindowPtr();
int RunApp(std::shared_ptr<App> app);
