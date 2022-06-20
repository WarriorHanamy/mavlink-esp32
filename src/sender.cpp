#include "UDPclient.cpp"

using namespace TestUDP;
namespace use{


class MocapSender
{

  static constexpr auto DEFAULT_BIND_HOST = "localhost";
  static constexpr auto DEFAULT_BIND_PORT = "14559";
  static constexpr auto DEFAULT_REMOTE_HOST = "192.168.129.2";
  static constexpr auto DEFAULT_REMOTE_PORT = "14550";
  //! Markers for broadcast modes. Not valid domain names.
  static constexpr auto BROADCAST_REMOTE_HOST = "***i want broadcast***";
  static constexpr auto PERMANENT_BROADCAST_REMOTE_HOST = "***permanent broadcast***";

private:
	UDPClient client_;
	mavlink::common::msg::ATT_POS_MOCAP pos;
public:
	MocapSender(boost::asio::io_service &io_service_);
	~MocapSender();

	void sendMocapMessage(int);
	void dataProcessing();
};

MocapSender::MocapSender(boost::asio::io_service &io_service):
	client_(io_service, DEFAULT_REMOTE_HOST, DEFAULT_REMOTE_PORT)
{
	SHOW("*****");
	dataProcessing();
}
MocapSender::~MocapSender()
{
}


void MocapSender::sendMocapMessage(int time = 1000){
	for(int i = 0; i < time; i++){
	boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	client_.send(&pos);
	}
}

void MocapSender::dataProcessing(){
	pos.x = 1 ;
	pos.y = 1;
	pos.z = 0;
}







}

int main(int argc, char **argv)
{
	//mpSender.sendMocapMessage();
	boost::asio::io_service io_service;
	use::MocapSender mpSender(io_service);
	mpSender.sendMocapMessage(atoi(argv[1]));
	// UDPClient client(io_service, hostIP, hostPort);
	// mavlink::common::msg::ATT_POS_MOCAP pos;


	// for(int i = 0; i < 1000; i++){
	// 	boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	// 	client.send(&pos);
	// }
	// SHOW(sizeof(mavlink::common::msg::ATT_POS_MOCAP));
	// client.send(&pos);


	
	// ip::address addr = ip::address::from_string("127.0.0.1");
	
	// SHOW(ip::address::from_string);

//    union {
//       short s;
//       char c[sizeof(short)];
//    }un;
	
//    un.s = 0x0102;
   
//    if (sizeof(short) == 2) {
//       if (un.c[0] == 1 && un.c[1] == 2)
//          printf("big-endian\n");
      
//       else if (un.c[0] == 2 && un.c[1] == 1)
//          printf("little-endian\n");
      
//       else
//          printf("unknown\n");
//    }
//    else {
//       printf("sizeof(short) = %d\n", sizeof(short));
//    }
// 	ip::udp::socket;
// 	ip::tcp::socket;
//    exit(0);

}




