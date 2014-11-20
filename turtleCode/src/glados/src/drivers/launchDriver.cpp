/**
* Low level functions for handling the motors of the launching system.
* Listens to:
*	
* Publishes on:
*	
*/ 
#include <ros/ros.h>
#include "switchSensor.h"

class LaunchDriver{
public:
	LaunchDriver();
	LaunchDriver(SwitchSensor *s, ros::NodeHandle h);
	bool checkSwitch();
	bool launch();
protected:
	SwitchSensor *sensor;
	ros::NodeHandle handle;
};

LaunchDriver::LaunchDriver () {
	sensor = new SwitchSensor("Rack");
	ros::NodeHandle nh;
	handle = nh;
} 

LaunchDriver::LaunchDriver(SwitchSensor *s, ros::NodeHandle h){
	sensor = s;
	handle = h;
}

bool LaunchDriver::checkSwitch(){
	sensor->read();
}

bool LaunchDriver::launch(){
	
}