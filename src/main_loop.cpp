#include "main_loop.h"

main_loop::main_loop()
{
}

main_loop::~main_loop()
{
}

void main_loop::run(std::function<void()> render, std::function<void()> update) {
	running = true;
	loopStartTime = SDL_GetTicks();
	accumulatedLag = 0.0f;
	double previous = loopStartTime;
	while (running) {
		double current = SDL_GetTicks();
		double elapsed = current - previous;
		previous = current;
		accumulatedLag += elapsed;
		while (accumulatedLag >= 16.6) {
			update();
			accumulatedLag -= 16.6;
		}
		render();
	}
}

void main_loop::pause() { }

void main_loop::stop() { }
