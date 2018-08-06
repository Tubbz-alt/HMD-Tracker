#include "TrackingApplication.hpp"

TrackingApplication::TrackingApplication(std::string const& thisIpAndPort, int desiredFps) : m_broadcaster(), m_desiredFps(desiredFps) {
	std::string hostAddress("tcp://" + thisIpAndPort);
	m_broadcaster.setHostAddress(hostAddress);

	std::cout << "[TrackingApplication.cpp] Initializing VR environment" << std::endl;
	vr::EVRInitError error(vr::VRInitError_None);
	m_vrSystem = vr::VR_Init(&error, vr::VRApplication_Background);

	if (error != vr::VRInitError_None) {
		std::cerr << "[TrackingApplication.cpp] Error while initializing VR environment" << std::endl;
		exit(1);
	}
	
}

void TrackingApplication::trackLoop() {

	const int numberOfTrackedDevices(vr::k_unMaxTrackedDeviceCount);
	vr::TrackedDevicePose_t trackedDevicesHandles[numberOfTrackedDevices];

	nanoseconds desiredFrameTimeNano((int) std::round((1.0 / m_desiredFps) * 1000000000));
	Clock::time_point lastTickTime(Clock::now());

	Clock::time_point lastPrintTime(Clock::now());
	int packetsSinceLastPrint(0);

	while (true) {

		if (Clock::now() - lastTickTime > desiredFrameTimeNano) {
			Message outboundMessageForThisFrame;
			int controllerCount(0);
			int baseStationCount(0);
			int trackerCount(0);
			float predictedSecondsFromNow(predictSecondsFromNow());
			m_vrSystem->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, predictedSecondsFromNow, trackedDevicesHandles, numberOfTrackedDevices);

			for (short i = 0; i < numberOfTrackedDevices; ++i) {

				vr::TrackedDeviceClass deviceType(vr::VRSystem()->GetTrackedDeviceClass(i));

				if (deviceType == vr::TrackedDeviceClass_HMD) {
					addHmdToMessage(trackedDevicesHandles[i], i, outboundMessageForThisFrame);
				}
				else if (deviceType == vr::TrackedDeviceClass_Controller) {
					addControllerToMessage(trackedDevicesHandles[i], i, controllerCount, outboundMessageForThisFrame);
					++controllerCount;
				}
				else if (deviceType == vr::TrackedDeviceClass_TrackingReference) {
					addBaseStationToMessage(trackedDevicesHandles[i], i, baseStationCount, outboundMessageForThisFrame);
					++baseStationCount;
				}
				else if (deviceType == vr::TrackedDeviceClass_GenericTracker) {
					addTrackerToMessage(trackedDevicesHandles[i], i, trackerCount, outboundMessageForThisFrame);
					++trackerCount;
				}

			}

			m_broadcaster.publish(outboundMessageForThisFrame);
			++packetsSinceLastPrint;

			// print number of messages every second
			std::chrono::high_resolution_clock::duration timeSinceLastPrint(Clock::now() - lastPrintTime);

			if (std::chrono::duration_cast<nanoseconds>(timeSinceLastPrint).count() > 1000000000) {
				std::cout << "[TrackingApplication.cpp] Published " << packetsSinceLastPrint << " packets in a second." << std::endl;
				packetsSinceLastPrint = 0;
				lastPrintTime = Clock::now();
			}

			lastTickTime = Clock::now();
		}

	}

}

float TrackingApplication::predictSecondsFromNow() const {
	float fSecondsSinceLastVsync;
	m_vrSystem->GetTimeSinceLastVsync(&fSecondsSinceLastVsync, nullptr);
	float fDisplayFrequency(m_vrSystem->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_DisplayFrequency_Float));
	float fFrameDuration(1.0f / fDisplayFrequency);
	float fVsyncToPhotons(m_vrSystem->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SecondsFromVsyncToPhotons_Float));
	return (fFrameDuration - fSecondsSinceLastVsync + fVsyncToPhotons);
}

void TrackingApplication::addHmdToMessage(vr::TrackedDevicePose_t const& pose, short deviceId, Message& message) const {
	TrackedDevice hmd;
	hmd.status = true;
	hmd.id = deviceId;
	vr::HmdMatrix34_t mat = pose.mDeviceToAbsoluteTracking;

	hmd.matrix[0] = mat.m[0][0];
	hmd.matrix[1] = mat.m[1][0];
	hmd.matrix[2] = mat.m[2][0];
	hmd.matrix[4] = mat.m[0][1];
	hmd.matrix[5] = mat.m[1][1];
	hmd.matrix[6] = mat.m[2][1];
	hmd.matrix[8] = mat.m[0][2];
	hmd.matrix[9] = mat.m[1][2];
	hmd.matrix[10] = mat.m[2][2];
	hmd.matrix[12] = mat.m[0][3];
	hmd.matrix[13] = mat.m[1][3];
	hmd.matrix[14] = mat.m[2][3];

	message.hmd = hmd;
}

void TrackingApplication::addControllerToMessage(vr::TrackedDevicePose_t const& pose, short deviceId, int controllerId, Message& message) const {
	ControllerDevice controller;
	controller.status = true;
	controller.id = deviceId;
	vr::HmdMatrix34_t mat = pose.mDeviceToAbsoluteTracking;

	controller.matrix[0] = mat.m[0][0];
	controller.matrix[1] = mat.m[1][0];
	controller.matrix[2] = mat.m[2][0];
	controller.matrix[4] = mat.m[0][1];
	controller.matrix[5] = mat.m[1][1];
	controller.matrix[6] = mat.m[2][1];
	controller.matrix[8] = mat.m[0][2];
	controller.matrix[9] = mat.m[1][2];
	controller.matrix[10] = mat.m[2][2];
	controller.matrix[12] = mat.m[0][3];
	controller.matrix[13] = mat.m[1][3];
	controller.matrix[14] = mat.m[2][3];

	vr::VRControllerState_t controllerState;
	if (!m_vrSystem->GetControllerState(deviceId, &controllerState, sizeof(vr::VRControllerState_t))) {
		return;
	}

	controller.trigger = controllerState.rAxis[1].x;

	if (controllerState.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) {
		controller.padTouchButton = true;
		controller.padX = controllerState.rAxis[0].x;
		controller.padY = controllerState.rAxis[0].y;
	}

	if (controllerState.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu)) {
		controller.appMenuButton = true;
	}

	if (controllerState.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Grip)) {
		controller.gripButton = true;
	}
	
	if (controllerState.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger)) {
		controller.triggerButton = true;
	}
	
	if (controllerState.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad)) {
		controller.padButton = true;
	}

	message.controllers[controllerId] = controller;
}


void TrackingApplication::addBaseStationToMessage(vr::TrackedDevicePose_t const& pose, short deviceId, int baseStationId, Message& message) const {
	TrackedDevice baseStation;
	baseStation.status = true;
	baseStation.id = deviceId;
	vr::HmdMatrix34_t mat = pose.mDeviceToAbsoluteTracking;

	baseStation.matrix[0] = mat.m[0][0];
	baseStation.matrix[1] = mat.m[1][0];
	baseStation.matrix[2] = mat.m[2][0];
	baseStation.matrix[4] = mat.m[0][1];
	baseStation.matrix[5] = mat.m[1][1];
	baseStation.matrix[6] = mat.m[2][1];
	baseStation.matrix[8] = mat.m[0][2];
	baseStation.matrix[9] = mat.m[1][2];
	baseStation.matrix[10] = mat.m[2][2];
	baseStation.matrix[12] = mat.m[0][3];
	baseStation.matrix[13] = mat.m[1][3];
	baseStation.matrix[14] = mat.m[2][3];

	message.baseStations[baseStationId] = baseStation;
}

void TrackingApplication::addTrackerToMessage(vr::TrackedDevicePose_t const& pose, short deviceId, int trackerId, Message& message) const {
	TrackedDevice tracker;
	tracker.status = true;
	tracker.id = deviceId;
	vr::HmdMatrix34_t mat = pose.mDeviceToAbsoluteTracking;

	tracker.matrix[0] = mat.m[0][0];
	tracker.matrix[1] = mat.m[1][0];
	tracker.matrix[2] = mat.m[2][0];
	tracker.matrix[4] = mat.m[0][1];
	tracker.matrix[5] = mat.m[1][1];
	tracker.matrix[6] = mat.m[2][1];
	tracker.matrix[8] = mat.m[0][2];
	tracker.matrix[9] = mat.m[1][2];
	tracker.matrix[10] = mat.m[2][2];
	tracker.matrix[12] = mat.m[0][3];
	tracker.matrix[13] = mat.m[1][3];
	tracker.matrix[14] = mat.m[2][3];

	message.trackers[trackerId] = tracker;
}