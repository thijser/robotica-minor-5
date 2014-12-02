#ifndef DRIVINGDRIVER_H
#define DRIVINGDRIVER_H

#include <ros/ros.h>
#include <threemxl/C3mxlROS.h>
#include <geometry_msgs/Twist.h>

class DrivingDriver{
protected:
	int ID;
	int currentPower;
	int state;	
	ros::NodeHandle handle;
	ros::Subscriber sub;

	std::string motor_port_name; 
	std::string motor_config_name;
	C3mxlROS* left_motor;
	C3mxlROS* right_motor;
	CXMLConfiguration motor_config_xml;
	CDxlConfig motor_config_left;
	CDxlConfig motor_config_right;

	double wheelDiameter;
	double wheelBase;
	void initMotors();
	void driveCallback(const geometry_msgs::Twist::ConstPtr &msg);

public:
	DrivingDriver() : handle("~") {
		init();
	}
	void init();
	int getID();
	int getState();
	bool setPower(int level);
	int getPower();
	bool drive();
	bool stop();
	void spin();
	int main(int argc, char **argv);


};

#endif 