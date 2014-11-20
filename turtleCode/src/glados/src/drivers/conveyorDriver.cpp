/**
* Low level functions for handling the motors of the conveyor belt.
* Listens to:
*	
* Publishes on:
*	
*/ 
#include <ros/ros.h>
#include "switchSensor.h"

class ConveyorDriver{
public:
	ConveyorDriver();
	ConveyorDriver(SwitchSensor *s, ros::NodeHandle h);
	bool checkSwitch();
	bool drive();
protected:
	SwitchSensor *sensor;
	ros::NodeHandle handle;
};

ConveyorDriver::ConveyorDriver () {
	sensor = new SwitchSensor("Mouth");
	ros::NodeHandle nh;
	handle = nh;
} 

ConveyorDriver::ConveyorDriver(SwitchSensor *s, ros::NodeHandle h){
	sensor = s;
	handle = h;
}

bool ConveyorDriver::checkSwitch(){
	sensor->read();
}

bool ConveyorDriver::drive(){
	
}
