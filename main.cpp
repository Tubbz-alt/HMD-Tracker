#include "TrackingApplication.hpp"

#include <iostream>

int main(int argc, char** argv) {

	if (argc != 2) {
		std::cerr << "[main.cpp] Usage: Application.exe HostIP:Port" << std::endl;
		exit(1);
	}

	TrackingApplication trackingApplication(argv[1]);
	trackingApplication.trackLoop();

	return 0;
}