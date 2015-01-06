#ifndef CONVEYORDRIVER_H
#define CONVEYORDRIVER_H

#include <ros/ros.h>
#include <std_msgs/Int16.h>

#include <iostream>
#include <fstream>
#include <string>

class ConveyorDriver{
public:
	ConveyorDriver();
	ConveyorDriver(ros::NodeHandle h);
	void init();
	int main(int argc, char **argv);
	void managerCallback(const std_msgs::Int16::ConstPtr &msg);
	void spin();
	std::fstream fs;
protected:
	
	ros::NodeHandle handle;
	ros::Subscriber sub;
	ros::Publisher pub;
};

#endif