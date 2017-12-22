#ifndef TRACKING_APPLICATION_HPP
#define TRACKING_APPLICATION_HPP

#include "Broadcaster.hpp"
#include "MessageStructs.hpp"
#include "openvr.h"

#include <array>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::nanoseconds nanoseconds;

class TrackingApplication {

	public:
		TrackingApplication(std::string const& thisIpAndPort, int desiredFps);

		void trackLoop();
		float predictSecondsFromNow() const;
		void addHmdToMessage(vr::TrackedDevicePose_t const& pose, short deviceId, Message& message) const;
		void addControllerToMessage(vr::TrackedDevicePose_t const& pose, short deviceId, int controllerId, Message& message) const;
		void addTrackerToMessage(vr::TrackedDevicePose_t const& pose, short deviceId, int trackerId, Message& message) const;

	private:
		Broadcaster m_broadcaster;
		vr::IVRSystem* m_vrSystem;
		int m_desiredFps;

};

#endif //TRACKING_APPLICATION_HPP
