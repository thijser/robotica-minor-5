#include <string>
#include "ros/ros.h"
#include <iostream>
#include <fstream>
#include "std_msgs/Int16.h"


int main(int argc, char **argv) {
	std::fstream fs;
	fs.open("/sys/class/gpio/export");
	fs << 20;  							// <<< PORT
	fs.close();

	fs.open("/sys/class/gpio/gpio20/direction"); //PORT 
	fs << "in";
	fs.close();

	ros::init(argc, argv, "endSwitchSensor");
	ros::NodeHandle handle;
	ros::Publisher pub = handle.advertise<std_msgs::Int16>("/tawi/sensors/endswitch", 100);
	std_msgs::Int16 msg;
	ros::Rate loop_Rate(5);

	while(ros::ok()){
		int stateRead = 2; 
		fs.open("/sys/class/gpio/gpio20/value");   // <<< PORT
		fs >> stateRead;
		fs.close();
		if(stateRead == 2)
			ROS_INFO("stateRead not set");
		msg.data = 1-stateRead; //Inverting for BBB logic
		
		pub.publish(msg);
		ROS_INFO("EndSwitch broadcasts %d", msg.data);
		loop_Rate.sleep();
	}
}