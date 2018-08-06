cmake_minimum_required (VERSION 2.6)
project(HMDTracker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

set(LIBZMQ_INCLUDE_DIR "" CACHE FILEPATH "Include directory of libzmq")
set(OPENVR_INCLUDE_DIR "" CACHE FILEPATH "Include directory of openvr")
set(LIBZMQ_LIBRARY_DIR "" CACHE FILEPATH "Library directory of libzmq")
set(OPENVR_LIBRARY_DIR "" CACHE FILEPATH "Library directory of openvr")

include_directories(${LIBZMQ_INCLUDE_DIR})
include_directories(${OPENVR_INCLUDE_DIR})

link_directories(${LIBZMQ_LIBRARY_DIR})
link_directories(${OPENVR_LIBRARY_DIR})

add_executable(HMDTracker
  main.cpp
  MessageStructs.hpp
  TrackingApplication.cpp
  TrackingApplication.hpp
  Broadcaster.cpp
  Broadcaster.hpp
)

target_link_libraries(HMDTracker "${LIBZMQ_LIBRARY_DIR}/libzmq-v141-mt-4_2_5.lib")
target_link_libraries(HMDTracker "${OPENVR_LIBRARY_DIR}/openvr_api.lib")
