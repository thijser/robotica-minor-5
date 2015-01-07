/**
* Low level functions for handling the motors of the conveyor belt.
* Listens to:
*	
* Publishes on:
*	
*/ 
#include "conveyorDriver.h"
#define ROTATIONTIME 20

ConveyorDriver::ConveyorDriver () {
	ros::NodeHandle nh;
	handle = nh;
} 

ConveyorDriver::ConveyorDriver(ros::NodeHandle h){
	handle = h;
}

void ConveyorDriver::init(){

	fs.open("/sys/class/gpio/export");
	fs << 50;  							// <<< PORT
	fs.close();
	fs.open("/sys/class/gpio/gpio50/direction"); //PORT 
   	fs << "out";
   	fs.close();
   	pub = handle.advertise<std_msgs::Int16>("/tawi/mngr/conveyor", 100);
	sub = handle.subscribe<std_msgs::Int16>("/tawi/motors/conveyor", 10, &ConveyorDriver::managerCallback, this);

}

void ConveyorDriver::managerCallback(const std_msgs::Int16::ConstPtr &msg){
	if(msg->data){
		fs.open("/sys/class/gpio/gpio50/value"); //PORT
	  	fs << "0"; // "1" for off
	   	fs.close();
	   	ROS_INFO("Sent 0 to gpio50/value");

	   	ros::Duration(ROTATIONTIME).sleep();
	   	std_msgs::Int16 msg;
	   	msg.data = 1;
	   	pub.publish(msg);
	   	fs.open("/sys/class/gpio/gpio50/value"); //PORT
	  	fs << "1"; // "1" for off
	   	fs.close();
	}
}

void ConveyorDriver::spin() {

	ros::Rate r(5);

	while(ros::ok()) {

		ros::spinOnce();
		r.sleep();
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "conveyorDriver");
	ROS_INFO("Started lconveyorDriver");
	ConveyorDriver cd;
	cd.init();
	cd.spin();

	return 0;
}