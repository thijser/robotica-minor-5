#ifndef NEWCORE_H
#define NEWCORE_H

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class NewCore{
public:
	void launchCallback(const std_msgs::String::ConstPtr &msg);
	void stopLaunch();
	void stopConvey();
	void startLaunch();
	void startConvey();
	void spin();
	void readSerial();
	void writeSerial(string shit);
	void askMath();
	void mathCallback(const std_msgs::String::ConstPtr &msg);
	int main(int argc, char **argv);
	void init();
protected:
	ros::NodeHandle handle;
	ros::Subscriber mngrSub;
	ros::Subscriber mathSub;
	ros::Publisher mngrPub;
	ros::Publisher mathPub;

	int ballCount = 0;

};
#endif 
