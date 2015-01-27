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
	int switch1_ok = 0;
	int  switch2_ok = 0;
	bool launching = false;
	bool launch();
	void setPort(int value);
	int main(int argc, char **argv);
	bool bSecondLaunch = false;
	bool stopLaunch = false;
	bool endTransition = false;
	int prevSwitch2 = 0;
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
	int ballCount = 0;
	int launchCount = 0;
};


#endif 
