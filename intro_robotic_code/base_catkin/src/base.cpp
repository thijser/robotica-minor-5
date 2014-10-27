/*
 * base.cpp
 *
 * Created on: 	Nov 17, 2013
 *     Author:	Floris Gaisser
 		echt?
 *
 * Versions:
 * 1.0		Initial version
 *  
 */

#include <threemxl/platform/io/configuration/XMLConfiguration.h>
#include <threemxl/dxlassert.h>
#include "base_catkin/base.h"
#include <base_catkin/BaseStatus.h>

#define CLIP(x, l, u) ((x)<(l))?(l):((x)>(u))?(u):(x)

void Base::init() {
	ROS_INFO("Initializing base");

	// Read parameters
	std::string motor_port_name, motor_config_name;

	ROS_ASSERT(nh_.getParam("motor_port", motor_port_name));
	ROS_ASSERT(nh_.getParam("motor_config", motor_config_name));
	ROS_ASSERT(nh_.getParam("wheel_diameter", wheel_diameter_));
	ROS_ASSERT(nh_.getParam("wheel_base", wheel_base_));

	// Subscriptions to command topic
	// code here!
	//for now, only velocity command
	vel_sub_ = nh_.subscribe<geometry_msgs::Twist>("/base/cmd_vel", 10, &Base::velocityCallback, this);
	torque_sub_ = nh_.subscribe<geometry_msgs::Twist>("/base/cmd_torque", 10, &Base::torqueCallback, this);
	// Publish status
	// code here!

	// Load motor configuration
	CXMLConfiguration motor_config_xml;
	ROS_ASSERT(motor_config_xml.loadFile(motor_config_name));

	CDxlConfig motor_config_left;
	motor_config_left.readConfig(motor_config_xml.root().section("left"));
	left_motor_ = new C3mxlROS(motor_port_name.c_str());
	left_motor_->setConfig(&motor_config_left);

	// Initialize left motor
	ros::Rate init_rate(1);
	while (ros::ok() && left_motor_->init() != DXL_SUCCESS) {
		ROS_WARN_ONCE("Couldn't initialize left wheel motor, will continue trying every second");
		init_rate.sleep();
	}

	CDxlConfig motor_config_right;
	motor_config_right.readConfig(motor_config_xml.root().section("right"));
	right_motor_ = new C3mxlROS(motor_port_name.c_str());
	right_motor_->setConfig(&motor_config_right);

	// Initialize right motor
	while (ros::ok() && right_motor_->init() != DXL_SUCCESS) {
		ROS_WARN_ONCE("Couldn't initialize right wheel motor, will continue trying every second");
		init_rate.sleep();
	}

	DXL_SAFE_CALL(left_motor_->set3MxlMode(motor_config_left.m3mxlMode));
	DXL_SAFE_CALL(right_motor_->set3MxlMode(motor_config_right.m3mxlMode));

	mode_pos_ = false;

	ROS_INFO("Base initialized");
}

void Base::spin() {
	ROS_INFO("Spinning");

	ros::Rate r(1000);

	while(ros::ok()) {
		ros::spinOnce();
		//publishStatus();
		r.sleep();
	}
}

/**
 * Callback that handles velocities
 */
void Base::velocityCallback(const geometry_msgs::Twist::ConstPtr &msg) {
	// Base is nonholonomic, warn if sent a command we can't execute
	if (msg->linear.y || msg->linear.z || msg->angular.x || msg->angular.y) {
		ROS_WARN("I'm afraid I can't do that, Dave.");
		return;
	}

	ROS_INFO("velocityCallback");

	if(mode_pos_) {
		left_motor_->set3MxlMode(SPEED_MODE);
		left_motor_->get3MxlMode();

		right_motor_->set3MxlMode(SPEED_MODE);
		right_motor_->get3MxlMode();

		mode_pos_ = false;
	}

	//calculate speed of left and right motor
	double v_x = msg->linear.x/(wheel_diameter_/2);
	double v_theta = msg->angular.z * (wheel_base_/wheel_diameter_);

	double v_motor_links = v_x - v_theta;
	double v_motor_rechts = v_x + v_theta;

	//set speed mode else it won't drive. bitch.
	left_motor_->set3MxlMode(SPEED_MODE);
	right_motor_->set3MxlMode(SPEED_MODE);

	//actually move motors
	left_motor_->setSpeed(v_motor_links);
	right_motor_->setSpeed(v_motor_rechts);	

	publishStatus();
}

void Base::torqueCallback(const geometry_msgs::Twist::ConstPtr &msg) {

	ROS_INFO("torqueCallback");

	left_motor_->set3MxlMode(TORQUE_MODE);
	right_motor_->set3MxlMode(TORQUE_MODE);

	left_motor_->setTorque(msg->linear.x - msg->angular.z);
	right_motor_->setTorque(msg->linear.x + msg->angular.z);

	publishStatus();
}

/**
 * Publish the status of the base motors
 */ 
void Base::publishStatus() {

	ros::NodeHandle handle;
	status_pub_ = handle.advertise<base_catkin::BaseStatus>("/base/status", 10);	
	base_catkin::BaseStatus currentStatus;

	//get states and stuff
	DXL_SAFE_CALL(left_motor_->getState());
	DXL_SAFE_CALL(right_motor_->getState());

	DXL_SAFE_CALL(left_motor_->getStatus());
	DXL_SAFE_CALL(right_motor_->getStatus());

	currentStatus.lspeed = left_motor_->presentSpeed();
	currentStatus.lposition = left_motor_->presentPos();
	currentStatus.ltorque = left_motor_->presentTorque();
	currentStatus.lstatus = left_motor_->presentStatus();

	currentStatus.rspeed = right_motor_->presentSpeed();
	currentStatus.rposition = right_motor_->presentPos();
	currentStatus.rtorque = right_motor_->presentTorque();
	currentStatus.rstatus = right_motor_->presentStatus();
	
	status_pub_.publish(currentStatus);
}
/**
 * Start the engines, ready,... set,... go!
 */
int main(int argc, char **argv)
{
	ros::init(argc, argv, "base");

	Base base;
	base.spin();

	return 0;
}



