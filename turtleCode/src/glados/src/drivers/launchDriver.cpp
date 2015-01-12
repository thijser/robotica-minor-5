/**
* Low level functions for handling the motors of the launching system.
* Listens to:
*	/tawi/motors/launch to std_msgs::Int16 (Launch when true)
 	/tawi/sensors/switch to std_msgs::Bool (Check if switch is ready)
* Publishes on:
*	
*/ 
#include "launchDriver.h"

using namespace std;

#define MAX_BALLS 20
#define HALF_BALLS 10

LaunchDriver::LaunchDriver () {
	ros::NodeHandle nh;
	handle = nh;
	LaunchDriver::init();
} 

LaunchDriver::LaunchDriver(ros::NodeHandle h){
	handle = h;
	LaunchDriver::init();
}

void LaunchDriver::init(){
	std::fstream fs;

	fs.open("/sys/class/gpio/export");
	fs << 60;  							// <<< PORT
	fs.close();
	fs.open("/sys/class/gpio/gpio60/direction"); //PORT 
   	fs << "out";
   	fs.close();
	switchSub = handle.subscribe<std_msgs::Int16>("/tawi/sensors/switch", 10, &LaunchDriver::switchCallback, this);
	endSwitchSub = handle.subscribe<std_msgs::Int16>("/tawi/sensors/endswitch", 10, &LaunchDriver::endSwitchCallback, this);
	pub = handle.advertise<std_msgs::Int16>("/tawi/mngr/launch", 100);
	launchSub = handle.subscribe<std_msgs::Int16>("/tawi/motors/launch", 10, &LaunchDriver::launchCallback, this);
	ballSub = handle.subscribe<std_msgs::Int16>("/tawi/core/ballcount", 10, &LaunchDriver::ballCallback, this);
	ROS_INFO("launchDriver subscribers initialised");
}

void LaunchDriver::endSwitchCallback(const std_msgs::Int16::ConstPtr &msg){
	switch2_ok = msg->data;
}

void LaunchDriver::ballCallback(const std_msgs::Int16::ConstPtr &msg){
	ballCount = msg->data;
}

void LaunchDriver::switchCallback(const std_msgs::Int16::ConstPtr &msg){
//	ROS_INFO("switchCallback");
	switch1_ok = msg->data;
}

void LaunchDriver::launchCallback(const std_msgs::Int16::ConstPtr &msg){
	ROS_INFO("launchCallback");
	if(msg->data == 1){
		launch();
	}
	else
		ROS_INFO("msg->data not 1");
}

bool LaunchDriver::launch(){
	//Good luck understanding this :D 
	//....fuck my life

	//rack has arrived at endswitch, but we're not full yet so we wait for more balls.
	if(ballCount < MAX_BALLS && launching && switch2_ok == 1){
		ROS_INFO("< MAX_BALLS && launch && switch2.ok");
		setPort(0);
		launching = false;
	}
	
	//This raises the platform!
	//We're full, time to release for the platform.
	if(ballCount >= MAX_BALLS && !launching && switch2_ok == 1){
		ROS_INFO("ballCount >= MAX_BALLS && launching && switch2_ok == 1)");

		setPort(1);
		launching = true;
		bSecondLaunch = true;
	}

	// safetycheck for stopping when not yet at halfballs
	if(!launching && switch1_ok == 1){
		ROS_INFO("!launch and switch1.ok");
		setPort(0);
	}
	
	// More than half-filled, lower the platform.
	if(ballCount >= HALF_BALLS && switch1_ok == 1 && !stopLaunch){
		ROS_INFO(">= HALF_BALLS && Switch1.ok");
		launching = true;
	}

	//State after done second launch. Waiting for Core to send new ballcount.
	if(ballCount >= HALF_BALLS && switch1_ok == 1 && stopLaunch){
		//Tell core that launcing is done.
		std_msgs::Int16 msg;
		msg.data = 0;
		pub.publish(msg);

		ROS_INFO(">= HALF_BALLS && Switch1.ok");
		setPort(0);
		launching = false;
	}

	//We launched twice. Time to stop launching and reset.
	if(ballCount < HALF_BALLS && switch1_ok){
		stopLaunch = false;
	}

	// State between the two switches. Set port 1 to continue to switch 2.
	if(launching && switch2_ok == 0){
		if(bSecondLaunch)
			stopLaunch = true;
		ROS_INFO("launching");
		setPort(1);
	}
}

void LaunchDriver::setPort(int value){
	ROS_INFO("Echoing %d to gpio60", value);
	//fs.open("/sys/class/gpio/gpio60/value"); // <<<PORT
	//fs << to_string(value); // "1" for off
	//fs.close();   	
}

void LaunchDriver::spin() {
	ROS_INFO("Spinning launchDriver");

	ros::Rate r(5);
	while(ros::ok()) {

		ros::spinOnce();
		r.sleep();
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "launchDriver");
	ROS_INFO("Started launchDriver");
	LaunchDriver ld;
	ld.spin();

	return 0;
}