#include "input.h"

Input::Input() { }
Input::~Input() { }

// InputEventNode& is a reference to an InputEventNode.  A copy of the next node
// is assigned to this reference arg and passed out by ref.
bool Input::DigestInputEvents(InputEventNode& event, bool first) {
	InputEventNode* eventPtr = nullptr;
	if (first) {
		eventPtr = _frameStart.get();
	} else {
		eventPtr = event.next.get();
	}
	if (eventPtr != nullptr) {
		// make a copy of the event at eventPtr to be passed out via setting event
		event = *(eventPtr);
		return true;
	}
	return false;
}

void Input::PushInputEvent(SDL_Event event) {
	auto node = std::make_shared<InputEventNode>();
	node->event = event;
	node->next = nullptr;
	if (_eventHead == nullptr) {
		_eventHead = node;
		_frameStart = node;
	} else {
		_eventHead->next = node;
		_eventHead = node;
	}
}

void Input::FlushEvents() {
	_eventHead = nullptr;
	_frameStart = nullptr;
	_frameEventCount = 0;
}
