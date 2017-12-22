#ifndef MESSAGE_STRUCTS_HPP
#define MESSAGE_STRUCTS_HPP

#include <array>

struct TrackedDevice {
	TrackedDevice() : status(false), id(-1) { 
		matrix.fill(0);
		matrix[0] = 1;
		matrix[5] = 1;
		matrix[10] = 1;
		matrix[15] = 1;
	};
	
	bool status;
	short id;
	std::array<float, 16> matrix;
};

struct ControllerDevice : TrackedDevice {
	
	ControllerDevice() : TrackedDevice(),
		appMenuButton(false), gripButton(false), padTouchButton(false), padButton(false),
		triggerButton(false), padX(0.0f), padY(0.0f), trigger(0.0f) {}
	
	bool appMenuButton;
	bool gripButton;
	bool padTouchButton;
	bool padButton;
	bool triggerButton;
	float padX;
	float padY;
	float trigger;

};

struct Message {

	Message() {};

	TrackedDevice hmd;
	std::array<ControllerDevice, 2> controllers;
	std::array<TrackedDevice, 2> trackers;
};

#endif // MESSAGE_STRUCTS_HPP