#ifndef NEWCORE_H
#define NEWCORE_H

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class NewCore{
public:
	void acceptBall();
	void launchCallback(const std_msgs::String::ConstPtr &msg);
	void stopLaunch();
	void stopConvey();
	void startLaunch();
	void startConvey();
	void spin();
	void deleteBall(const int ballnumber);
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
	ros::Publisher nmbrPub;
	ros::Publisher mathPub;
	ros::Publisher ballPub;
	int ballCount = 0;

};
#endif 
