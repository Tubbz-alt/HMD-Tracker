#ifndef MESSAGE_SENDER_HPP
#define MESSAGE_SENDER_HPP

#include "MessageStructs.hpp"
#include "zmq.hpp"

class Broadcaster {

	public:
		Broadcaster();
		void setHostAddress(std::string const& hostAddress);
		void publish(Message const& msg);

    private:
		zmq::context_t m_context;
		zmq::socket_t m_socket;
		size_t m_messageLength;
};

#endif // MESSAGE_SENDER_HPP