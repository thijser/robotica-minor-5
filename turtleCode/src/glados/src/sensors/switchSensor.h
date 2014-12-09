#ifndef SWITCHSENSOR_H
#define SWITCHSENSOR_H

#include <string>
#include "ros/ros.h"
#include <iostream>
#include <fstream>
#include "std_msgs/Int16.h"

class SwitchSensor{
protected:
	ros::NodeHandle handle;
	int state;
	std::string tag;	
	int readParse();
public:
	SwitchSensor();
	SwitchSensor(std::string tag);
	int read();
	bool setState(int s);
	int getState();
	std::string getTag();
	void spin();
	int main(int argc, char **argv);

};

#endif