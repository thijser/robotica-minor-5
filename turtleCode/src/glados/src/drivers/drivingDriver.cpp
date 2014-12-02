/*
Uses the sound_play library of ROS to play sound on the OS' default sound driver. 
Listening on:
	/tawi/motors/wheels to geometry_msgs::Twist (directions to drive)
Publishing on:

*/
#include <threemxl/platform/io/configuration/XMLConfiguration.h>
#include <threemxl/dxlassert.h>
#include <ros/ros.h>
#include <threemxl/C3mxlROS.h>
#include <geometry_msgs/Twist.h>
#include "drivingDriver.h"

DrivingDriver::DrivingDriver() {
	DrivingDriver(99999);
}

DrivingDriver::DrivingDriver(int id){
	ros::NodeHandle handle("~");
	ID = id;
	sub = handle.subscribe<geometry_msgs::Twist>("/base/cmd_vel", 10, &DrivingDriver::driveCallback, this);

	//Parameters set in launch file.
	ROS_ASSERT(handle.getParam("motor_port", motor_port_name));
	ROS_ASSERT(handle.getParam("motor_config", motor_config_name));
	ROS_ASSERT(handle.getParam("wheel_diameter", wheelDiameter));
	ROS_ASSERT(handle.getParam("wheel_base", wheelBase));

	ROS_ASSERT(motor_config_xml.loadFile(motor_config_name));

	initMotors();
	
	DXL_SAFE_CALL(left_motor->set3MxlMode(motor_config_left.m3mxlMode));
	DXL_SAFE_CALL(right_motor->set3MxlMode(motor_config_right.m3mxlMode));
}

void DrivingDriver::driveCallback(const geometry_msgs::Twist::ConstPtr &msg){

	ROS_INFO("driveCallback");
	double v_x = msg->linear.x/(wheelDiameter/2);
	double v_theta = msg->angular.z * (wheelBase/wheelDiameter);

	double leftMotorVelocity = v_x - v_theta;
	double rightMotorVelocity = v_x + v_theta;

	left_motor->set3MxlMode(SPEED_MODE);
	left_motor->setSpeed(leftMotorVelocity);
	right_motor->set3MxlMode(SPEED_MODE);
	right_motor->setSpeed(rightMotorVelocity);

}

int DrivingDriver::getID() {
	return ID;
}


int DrivingDriver::getState(){
	return state;
}

bool DrivingDriver::setPower(int level){
	currentPower = level;
}

int DrivingDriver::getPower(){
	return currentPower;
}

bool DrivingDriver::drive(){
	
	return true;
}

bool DrivingDriver::stop() {
	//dingen
	return false;
}

void DrivingDriver::initMotors(){
	ros::Rate init_rate(1);
	
	motor_config_left.readConfig(motor_config_xml.root().section("left"));
	left_motor = new C3mxlROS(motor_port_name.c_str());
	left_motor->setConfig(&motor_config_left);

	while (ros::ok() && left_motor->init() != DXL_SUCCESS) {
		ROS_WARN_ONCE("Couldn't initialize left wheel motor, will continue trying every second");
		init_rate.sleep();
	}

	motor_config_right.readConfig(motor_config_xml.root().section("right"));
	right_motor = new C3mxlROS(motor_port_name.c_str());
	right_motor->setConfig(&motor_config_right);

	while (ros::ok() && right_motor->init() != DXL_SUCCESS) {
		ROS_WARN_ONCE("Couldn't initialize right wheel motor, will continue trying every second");
		init_rate.sleep();
	}
}

void DrivingDriver::spin() {
	ROS_INFO("Spinning");

	ros::Rate r(1000);

	while(ros::ok()) {
		ros::spinOnce();
		r.sleep();
	}
}

int main(int argc, char **argv)
{

	ros::init(argc, argv, "drivingDriver");
	DrivingDriver dd;
	dd.spin();

	return 0;
}