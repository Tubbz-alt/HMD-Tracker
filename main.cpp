#include "TrackingApplication.hpp"

#include <iostream>
#include <limits.h>

int main(int argc, char** argv) {

	if (argc != 2 && argc != 3) {
		std::cerr << "[main.cpp] Usage: Application.exe HostIP:Port [DesiredFps]" << std::endl;
		exit(1);
	}

	std::string hostIp(argv[1]);

	int desiredFps(INT_MAX);
	if (argc == 3) {
		desiredFps = std::stoi(argv[2]);
	}

	TrackingApplication trackingApplication(hostIp, desiredFps);
	trackingApplication.trackLoop();

	return 0;
}