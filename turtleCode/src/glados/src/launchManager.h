#ifndef LAUNCHDRIVER_H
#define LAUNCHDRIVER_H

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <string.h>
class LaunchManager{
public:
	void coreCallback(const std_msgs::String::ConstPtr &msg);
	void launchCallback(const std_msgs::Int16::ConstPtr & msg);
	void conveyCallback(const std_msgs::Int16::ConstPtr & msg);
	void init();
	void spin();
protected:
	ros::NodeHandle handle;
	ros::Subscriber coreSub;
	ros::Subscriber launchSub;
	ros::Subscriber conveySub;
	ros::Publisher corePub;
	ros::Publisher launchPub;
	ros::Publisher conveyPub;
	bool launching = false;
	bool conveying = false;

};

#endif 