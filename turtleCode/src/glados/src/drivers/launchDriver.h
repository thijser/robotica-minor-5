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
	bool switch1_ok;
	bool switch2_ok;
	bool launching;
	bool launch();
	void setPort(int value);
	int main(int argc, char **argv);
	void switchCallback(const std_msgs::Int16::ConstPtr &msg);
	void endSwitchCallback(const std_msgs::Int16::ConstPtr &msg);
	void launchCallback(const std_msgs::Int16::ConstPtr &msg);
	void ballCallback(const std_msgs::Int16::ConstPtr &msg);
	void spin();
protected:
	std::fstream fs;
	ros::NodeHandle handle;
	ros::Subscriber switchSub;
	ros::Subscriber endSwitchSub;
	ros::Subscriber launchSub;	
	ros::Publisher pub;
	ros::Subscriber ballSub;
	int ballCount;
	int launchCount = 0;
};


#endif 
