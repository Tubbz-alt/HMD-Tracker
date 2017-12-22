# HMD-Tracker
Application for retrieving tracking information of head-mounted displays and their accessories 
from SteamVR and publishing them into the network using ZeroMQ. 

## Dependencies
 * libzmq: https://github.com/zeromq/libzmq
 * openvr: https://github.com/ValveSoftware/openvr

## Build Instructions
During the configuration process of the project with [CMake](https://cmake.org/), you will be asked to provide the include and library directories
of libzmq and openvr. After generating and building the project, ensure that both dynamic library files can be found by the application. This
is done by placing them either in the directory of the application itself or in a directory in your system's PATH variable.

## Usage
```HMDTracker.exe HostIP:Port [DesiredFps]```
 * HostIP:Port is a string containing the IP address of the machine on which the application is running and the port on which messages will be published
 * DesiredFps is an optional integer used to restrict the frequency with which packets will be published
 
Example: ```HMDTracker.exe 29.4.93.1:7777 90``` - a 90Hz tracking update on port 7777, running on machine 29.4.93.1

## Receiving data in another application
The message structure used by the application is defined in ```MessageStructs.hpp```. Each ```Message``` object contains information on the HMD, two controllers and two
trackers (base stations). In another application, a network packet can be received and parsed as follows:

```c
#include "zmq.hpp"
#include "MessageStructs.hpp"

[...]

zmq::context_t context(1);
zmq::socket_t socket(context, ZMQ_SUB);

while(true) {
    zmq::message_t zmqMessage(sizeof(Message));
    socket.recv(&zmqMessage);
    Message message;
    std::memcpy(&message, reinterpret_cast<Message*>(zmqMessage.data()), sizeof(Message));
    
    // do something with the message
}
```