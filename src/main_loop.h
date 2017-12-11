#include <stdlib.h>
#include <iostream>
#include <SDL.h>
#include <functional>
#pragma once
class main_loop
{
private:
	bool running = false;
	double loopStartTime = 0.0f;
	double accumulatedLag = 0.0f;
public:
	main_loop();
	~main_loop();
	void run(
		std::function<void()> render,
		std::function<void()> update
	);
	void pause();
	void stop();
};
