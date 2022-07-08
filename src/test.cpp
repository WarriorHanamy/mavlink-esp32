#include "UDPclient.cpp"
#include <boost/timer.hpp>

using namespace TestUDP;
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
	uint8_t buf[36+1+5+2];
public:
	MocapSender(boost::asio::io_service &io_service_);
	~MocapSender();

	void sendMocapMessage(int);
	void dataProcessing();

	void serialization();
	void test();
	void justSerialization(mavlink::mavlink_message_t &p);
	
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
	test();
	boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	client_.send(buf);
	}
}


void MocapSender::dataProcessing(){
	pos.time_usec = 0xFFFFFFFFFFFFFFFF;
	pos.q = {100.23,100.23,100.23,100.23};
	pos.x = 100.23;
	pos.y = 100.23;
	pos.z = 100.23;
}


void MocapSender::justSerialization(mavlink::mavlink_message_t &p){
	p.magic = MAVLINK_STX_MAVLINK1;
	p.len = pos.MIN_LENGTH;
	p.seq = 100;
	p.sysid = 100;
	p.compid = 100;
	p.msgid = 138;

}


namespace exHelp{
	using namespace mavlink;

// MAVLINK_HELPER uint16_t mavlink_finalize_message_buffer(mavlink_message_t* msg, uint8_t system_id, uint8_t component_id,
// 						      mavlink_status_t* status, uint8_t min_length, uint8_t length, uint8_t crc_extra)
// {

// 	mavlink::mavlink_finalize_message(&p,1,1,pos.MIN_LENGTH,pos.LENGTH,0);
// 		uint8_t signature_len, header_len;
// 		uint8_t *ck;
//         uint8_t length = p.len;

// 	// msg->checksum = checksum;

// 	// if (signing) {
// 	// 	mavlink_sign_packet(status->signing,
// 	// 			    msg->signature,
// 	// 			    (const uint8_t *)buf, header_len,
// 	// 			    (const uint8_t *)_MAV_PAYLOAD(msg), msg->len,
// 	// 			    (const uint8_t *)_MAV_PAYLOAD(msg)+(uint16_t)msg->len);
// 	// }
	
// 	// return msg->len + header_len + 2 + signature_len;
// }
}

#define SHOWLINE( x ) std::cout << (x) << std::endl;
void MocapSender::test(){
	using namespace mavlink;
	mavlink::mavlink_message_t p;
	mavlink::MsgMap map(&p);
	pos.serialize(map);
	std::cout << "MAGIC" << std::hex << (int) p.magic << std::endl;
	justSerialization(p);
	
	
	std::cout << "MAGIC" << std::hex << (int) p.magic << std::endl;
	uint8_t header_len = MAVLINK_CORE_HEADER_MAVLINK1_LEN+1;
	uint8_t *ck = buf + 1 + header_len + p.len ;
	if (p.magic == MAVLINK_STX_MAVLINK1) {
		buf[0] = p.magic;
		buf[1] = p.len;
		buf[2] = p.seq;
		buf[3] = p.sysid;
		buf[4] = p.compid;
		buf[5] = p.msgid & 0xFF;
		memcpy(&buf[6], _MAV_PAYLOAD(&p), p.len);
	}
	uint16_t checksum = crc_calculate(&buf[1], header_len-1);
	crc_accumulate_buffer(&checksum, _MAV_PAYLOAD(&p), p.len);
	// crc_accumulate(crc_extra, &checksum)
	

	std::ostream& stm = std::cout << std::hex << std::setw(2)<<std::setfill('0');
	for(auto &v : buf)
	stm << (int) v;

	std::cout << "Length p " << (int)p.len << std::endl;
	std::cout << sizeof(float) << std::endl;

	std:: cout << (int) buf[0] << std::endl;
	SHOWLINE(sizeof(unsigned long int))
	SHOWLINE(sizeof(pos.time_usec))
	SHOWLINE((int)p.len)

	std::cout << p.payload64<<std::endl;
	
	std::cout << "STX" << (int) buf[0] << std::endl;

	

}



#include <bitset>

int main(int argc, char **argv)
{
	//mpSender.sendMocapMessage();
	boost::asio::io_service io_service;
	MocapSender mpSender(io_service);

	boost::timer t;
	// mpSender.serialization();
	// mpSender.test();
	mpSender.sendMocapMessage();

	// float flt = 4.0f;
	// std::bitset<32> x(flt) ;
	// std::cout << "Float Store" <<  x << std::endl;
	// switch (argc)
	// {
	// case 1:
	// 	mpSender.sendMocapMessage();
	// 	break;
	// case 2:
	// 	mpSender.sendMocapMessage(atoi(argv[1]));
	// 	break;
	// default:
	// 	SHOW("too many arguments")
	// 	break;
	// }

	// SHOW("time elapsed")
	// SHOW(t.elapsed())
	
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




