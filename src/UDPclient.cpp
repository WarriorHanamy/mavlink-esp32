#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <mavconn/interface.h>
#include <stdlib.h>
#include <boost/chrono.hpp>
#define hostIP "192.168.129.2"
#define hostPort "14550"
#define bindIP "127.0.0.1"
#define bindPort "14559"
namespace TestUDP{
using boost::asio::ip::udp;
using namespace boost::asio;
// source gode in https://gist.github.com/kaimallea/e112f5c22fe8ca6dc627  
// modified by Erchao Rong 
// just for first version, ultimately we will use libmavconn interface
class UDPClient
{
public:
	UDPClient(
		boost::asio::io_service& io_service, 
		const std::string& host, 
		const std::string& port
	) : io_service_(io_service), socket_(io_service, udp::endpoint(udp::v4(), 0)) {
		udp::resolver resolver(io_service_);
		udp::resolver::query query(udp::v4(), host, port);
		udp::resolver::iterator iter = resolver.resolve(query);
		endpoint_ = *iter;

		// socket_.set_option(udp::socket::reuse_address(true));
		// ip::udp::endpoint bindEP(ip::address::from_string(bindIP),atoi(bindPort));
		// socket_.bind(bindEP);
	}

	~UDPClient()
	{
		socket_.close();
	}

	void send(const std::string& msg) {
		socket_.send_to(boost::asio::buffer(msg, msg.size()), endpoint_);
	}

	void send(mavlink::common::msg::ATT_POS_MOCAP *pos){
		socket_.send_to(boost::asio::const_buffers_1(pos, 128), endpoint_);
	}



private:
	boost::asio::io_service& io_service_;
	udp::socket socket_;
	udp::endpoint endpoint_;
};


}

#define SHOW( X ) std::cout<<(X)<<std::endl;








