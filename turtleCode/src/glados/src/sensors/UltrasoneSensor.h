#ifndef ULTRASONESENSOR_H
#define ULTRASONESENSOR_H

#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <string>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

class UltrasoneSensor{
public:
	void spin();
	int init(void);
protected:
	ros::NodeHandle handle ;
	ros::Publisher pub;
	unsigned int * pruData;
};

#endif
