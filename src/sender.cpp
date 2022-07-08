#include "UDPclient.cpp"
#include <ros/ros.h>
#include <eigen_conversions/eigen_msg.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TransformStamped.h>
#include <mavros/frame_tf.h>
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
	ros::NodeHandle mpNdHandle_;
	ros::Subscriber mpPoseSub_;
	// ros::Subscriber mpTfSub_;
public:
	MocapSender(boost::asio::io_service &io_service_);
	~MocapSender();

	void sendMocapMessage();
	void dataProcessing(const geometry_msgs::PoseStamped::ConstPtr &pose);

// intialize will create a subscribe thread to deal with subscribtion
	void intialize();
	
};

MocapSender::MocapSender(boost::asio::io_service &io_service):
	client_(io_service, DEFAULT_REMOTE_HOST, DEFAULT_REMOTE_PORT)
{
	SHOW("*****");
	// dataProcessing();
}
MocapSender::~MocapSender()
{
}

void MocapSender::sendMocapMessage()
{
	client_.send(&pos);
}

void MocapSender::dataProcessing(const geometry_msgs::PoseStamped::ConstPtr &pose){
	ROS_INFO("get data %d", pose->header.seq);
	Eigen::Quaterniond q_enu;
	using namespace mavros;
	tf::quaternionMsgToEigen(pose->pose.orientation, q_enu);
	auto q = ftf::transform_orientation_enu_ned(
		ftf::transform_orientation_baselink_aircraft(q_enu));

	auto position = ftf::transform_frame_enu_ned(
		Eigen::Vector3d(
			pose->pose.position.x,
			pose->pose.position.y,
			pose->pose.position.z));

	pos.time_usec = pose->header.stamp.toNSec() / 1000;
	ftf::quaternion_to_mavlink(q, pos.q);
	pos.x = position.x();
	pos.y = position.y();
	pos.z = position.z();

	sendMocapMessage();
}

void MocapSender::intialize(){
		// bool use_tf;
		// bool use_pose;

		mpPoseSub_ = mpNdHandle_.subscribe("/chatter", 1, &MocapSender::dataProcessing, this);

		ros::spin();
		/** @note For VICON ROS package, subscribe to TransformStamped topic */
		// mpNdHandle_.param("use_tf", use_tf, false);

		/** @note For Optitrack ROS package, subscribe to PoseStamped topic */
		// mpNdHandle_.param("use_pose", use_pose, true);

		// if (use_tf && !use_pose) {
		// 	mocap_tf_sub = mp_nh.subscribe("tf", 1, &MocapPoseEstimatePlugin::mocap_tf_cb, this);
		// }
		// else if (use_pose && !use_tf) {
		// 	mocap_pose_sub = mp_nh.subscribe("pose", 1, &MocapPoseEstimatePlugin::mocap_pose_cb, this);
		// }
		// else {
		// 	ROS_ERROR_NAMED("mocap", "Use one motion capture source.");
		// }
		// if (use_tf && !use_pose) {
		// 	// mpPoseSub_ = mpNdHandle_.subscribe("tf", 1, &MocapSender::dataProcessing, this);
		// }
		// else if (use_pose && !use_tf) {
		// 	mpPoseSub_ = mpNdHandle_.subscribe("tf", 1, &MocapSender::dataProcessing, this);
		// 	// mpTfSub_ = mpNdHandle_.subscribe("pose", 1, &MocapPoseEstimatePlugin::mocap_pose_cb, this);
		// }
		// else {
		// 	ROS_ERROR_NAMED("mocap", "Use one motion capture source.");

}



}


int main(int argc, char **argv)
{
	//mpSender.sendMocapMessage();
	ros::init(argc,argv,"MotionCaptureSender");
	boost::asio::io_service io_service;
	use::MocapSender mpSender(io_service);
	// mavlink::mavlink_message_t
	// mpSender.intialize();

	
	// mpSender.sendMocapMessage(atoi(argv[1]));

	//mavlink_msg_attitude_pack(1, 200, &msg, microsSinceEpoch(), 1.2, 1.7, 3.14, 0.01, 0.02, 0.03);


}




