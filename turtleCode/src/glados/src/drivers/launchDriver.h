#ifndef LAUNCHDRIVER_H
#define LAUNCHDRIVER_H

#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int16.h>

#include <iostream>
#include <fstream>
#include <string>


class LaunchDriver{
public:
	LaunchDriver();
	LaunchDriver(ros::NodeHandle h);
	void init();
	bool checkSwitch();
	bool launch();
	int main(int argc, char **argv);
	void switchCallback(const std_msgs::Int16::ConstPtr &msg);
	void launchCallback(const std_msgs::Int16::ConstPtr &msg);
	void spin();
protected:
	std::fstream fs;
	bool switchReady = false;
	ros::NodeHandle handle;
	ros::Subscriber switchSub;
	ros::Subscriber launchSub;	
	ros::Publisher pub;
};


#endif 
