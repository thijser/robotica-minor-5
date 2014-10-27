/**
Software License Agreement (BSD)

\authors   Mike Purvis <mpurvis@clearpathrobotics.com>
\copyright Copyright (c) 2014, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the
   following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
   following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of Clearpath Robotics nor the names of its contributors may be used to endorse or promote
   products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WAR-
RANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, IN-
DIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "geometry_msgs/Twist.h"
#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "sensor_msgs/JoyFeedbackArray.h"
#include "teleop_twist_joy/teleop_twist_joy.h"
#include <sensor_msgs/JointState.h>


namespace teleop_twist_joy
{
	/**
	 * Internal members of class. This is the pimpl idiom, and allows more flexibility in adding
	 * parameters later without breaking ABI compatibility, for robots which link TeleopTwistJoy
	 * directly into base nodes.
	 */
	struct TeleopTwistJoy::Impl
	{
	  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
	  void publishTorque(const sensor_msgs::Joy::ConstPtr& joy_msg);

	  ros::Subscriber joy_sub;
	  ros::Publisher cmd_torque_pub;
	  ros::Publisher cmd_vel_pub;

	  int toggle_button;
	  int prev_toggle_button;
	  int enable_button;
	  int prev_enable_button;
	  int enable_turbo_button;
	  int axis_linear;
	  int axis_angular;
	  double scale_linear;
	  double scale_linear_turbo;
	  double scale_angular;

	  bool allow_movement;
	  bool torquemode;
	  bool sent_disable_msg;
	};

	/**
	 * Constructs TeleopTwistJoy.ros1/name
	 * \param nh NodeHandle to use for setting up the publisher and subscriber.
	 * \param nh_param NodeHandle to use for searching for configuration parameters.
	 */
	TeleopTwistJoy::TeleopTwistJoy(ros::NodeHandle* nh, ros::NodeHandle* nh_param)
	{
	  	pimpl_ = new Impl;
	  	pimpl_->allow_movement = false;
	  	pimpl_->torquemode = false;

	  	pimpl_->cmd_vel_pub = nh->advertise<geometry_msgs::Twist>("/base/cmd_vel", 10, true);
	  	pimpl_->joy_sub = nh->subscribe<sensor_msgs::Joy>("joy", 1, &TeleopTwistJoy::Impl::joyCallback, pimpl_);
	  	pimpl_->cmd_torque_pub = nh->advertise<geometry_msgs::Twist>("base/cmd_torque", 10, true);

	  	nh_param->param<int>("enable_button", pimpl_->enable_button, 1);
	  	nh_param->param<int>("toggle_button", pimpl_->toggle_button, 1);
	  	nh_param->param<int>("enable_turbo_button", pimpl_->enable_turbo_button, -1);

	  	nh_param->param<int>("axis_linear", pimpl_->axis_linear, 1);
	  	nh_param->param<double>("scale_linear", pimpl_->scale_linear, 1);
	  	nh_param->param<double>("scale_linear_turbo", pimpl_->scale_linear_turbo, 1);

	  	nh_param->param<int>("axis_angular", pimpl_->axis_angular, 1);
	  	nh_param->param<double>("scale_angular", pimpl_->scale_angular, 1);

	  	ROS_INFO_NAMED("TeleopTwistJoy", "Using axis %i for linear and axis %i for angular.",
	      	pimpl_->axis_linear, pimpl_->axis_angular);
	  	ROS_INFO_NAMED("TeleopTwistJoy", "Teleop on button %i at scale %f linear, scale %f angular.",
	      	pimpl_->enable_button, pimpl_->scale_linear, pimpl_->scale_angular);
	  	ROS_INFO_COND_NAMED(pimpl_->enable_turbo_button >= 0, "TeleopTwistJoy",
	      	"Turbo on button %i at scale %f linear.", pimpl_->enable_turbo_button, pimpl_->scale_linear_turbo);

	  	pimpl_->sent_disable_msg = false;
	}

	void TeleopTwistJoy::Impl::joyCallback(const sensor_msgs::Joy::ConstPtr& joy_msg)
	{	
		//Check trigger-toggles
		if(joy_msg->buttons[enable_button] > prev_enable_button) 
			allow_movement = !allow_movement;
		prev_enable_button = joy_msg->buttons[enable_button];

		if(joy_msg->buttons[toggle_button] > prev_toggle_button) 
			torquemode = !torquemode;
		prev_toggle_button = joy_msg->buttons[toggle_button];

		if(torquemode) {
			publishTorque(joy_msg);
			return;
		}
	  	// Initializes with zeros by default.
	 	geometry_msgs::Twist cmd_vel_msg;
	  	//ROS_INFO("prev: %d || current: %d", prev_enable_button, joy_msg->buttons[enable_button]);
	  
	  	//Standard analog stick driving
	  	//ROS_INFO("%d", joy_msg->buttons[enable_turbo_button]);
	  	if (/*enable_turbo_button >= 0 && */joy_msg->buttons[enable_turbo_button])
	  	{
		  	ROS_INFO("turbo");
		    cmd_vel_msg.linear.x = joy_msg->axes[axis_linear] * scale_linear_turbo;
		    cmd_vel_msg.angular.z = joy_msg->axes[axis_angular] * scale_angular;
		    cmd_vel_pub.publish(cmd_vel_msg);
		    sent_disable_msg = false;
	  	}
	  	else if (allow_movement)
	  	{
		  	ROS_INFO("enabled button");
		    cmd_vel_msg.linear.x = joy_msg->axes[axis_linear] * scale_linear;
		    cmd_vel_msg.angular.z = joy_msg->axes[axis_angular] * scale_angular;
		    cmd_vel_pub.publish(cmd_vel_msg);
		    sent_disable_msg = false;
	  	}
	  	else
	  	{
		    // When enable button is released, immediately send a single no-motion command
		    // in order to stop the robot.
			if (!sent_disable_msg)
		    {
		    	ROS_INFO("button release");
		      cmd_vel_pub.publish(cmd_vel_msg);
		      sent_disable_msg = true;;
		    }
	  	}
	}

	void TeleopTwistJoy::Impl::publishTorque(const sensor_msgs::Joy::ConstPtr& joy_msg) {
		geometry_msgs::Twist cmd_torque_msg;

		cmd_torque_msg.linear.x = joy_msg->axes[axis_linear] / 100;
		cmd_torque_msg.angular.z = joy_msg->axes[axis_angular] / 100;

		cmd_torque_pub.publish(cmd_torque_msg);
	}
}  // close namespace teleop_twist_joy

int main(int argc, char **argv) {


	return 0;
}