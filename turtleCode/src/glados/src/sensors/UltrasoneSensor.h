#ifndef ULTRASONESENSOR_H
#define ULTRASONESENSOR_H

#include <string>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

class UltrasoneSensor{
public:
	void spin();
	int init(void);
	UltrasoneSensor();
protected:
	ros::NodeHandle;
	ros::Publisher;
};

#endif
