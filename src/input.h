#pragma once
#include <SDL.h>
#include <memory>
#include <iostream>

struct InputEventNode {
	SDL_Event event;
	std::shared_ptr<InputEventNode> next = nullptr;
	InputEventNode() { }
	~InputEventNode() { }
};

class Input {
public:
	Input();
	~Input();
	bool DigestInputEvents(InputEventNode& event, bool first);
	void PushInputEvent(SDL_Event event);
	void FlushEvents();
private:
	int _frameEventCount = 0;
	std::shared_ptr<InputEventNode> _frameStart = nullptr;
	std::shared_ptr<InputEventNode> _eventHead = nullptr;
};