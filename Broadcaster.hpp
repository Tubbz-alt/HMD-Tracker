#ifndef MESSAGE_SENDER_HPP
#define MESSAGE_SENDER_HPP

#include "MessageStructs.hpp"
#include "zmq.hpp"

#include <csignal>

class Broadcaster {

	public:
		Broadcaster();
		void setHostAddress(std::string const& hostAddress);
		void publish(Message const& msg);
		
        static void signalHandler(int signal);
		static zmq::context_t context;
		static zmq::socket_t socket;
		static size_t messageLength;
};

#endif // MESSAGE_SENDER_HPP
