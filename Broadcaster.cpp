#include "Broadcaster.hpp"

zmq::context_t Broadcaster::context(1);
zmq::socket_t Broadcaster::socket(Broadcaster::context, ZMQ_PUB);
size_t Broadcaster::messageLength(sizeof(Message));

Broadcaster::Broadcaster() {	
	std::signal(SIGINT, Broadcaster::signalHandler);
}

void Broadcaster::setHostAddress(std::string const& hostAddress) {
	Broadcaster::socket.bind(hostAddress.c_str());
}

void Broadcaster::publish(Message const& message) {
	zmq::message_t zmqMessage(Broadcaster::messageLength);
	memcpy(zmqMessage.data(), &message, Broadcaster::messageLength);
	Broadcaster::socket.send(zmqMessage);
}

void Broadcaster::signalHandler(int signal) {
	Broadcaster::socket.close();
	Broadcaster::context.close();
	exit(0);
}
