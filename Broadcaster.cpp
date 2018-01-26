#include "Broadcaster.hpp"

Broadcaster::Broadcaster() :
  m_context(1), m_socket(m_context, ZMQ_PUB), m_messageLength(sizeof(Message)) {

	// set maximum number of outstanding messages in the queue
	int conflateMessages(1); 
	m_socket.setsockopt(ZMQ_CONFLATE, &conflateMessages, sizeof(conflateMessages));
}

void Broadcaster::setHostAddress(std::string const& hostAddress) {
	m_socket.bind(hostAddress.c_str());
}

void Broadcaster::publish(Message const& message) {
	zmq::message_t zmqMessage(m_messageLength);
	memcpy(zmqMessage.data(), &message, m_messageLength);
	m_socket.send(zmqMessage);
}
