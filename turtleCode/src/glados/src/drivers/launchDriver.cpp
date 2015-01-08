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
	
	//!launch and switch1.ok
	//	echo 0
	if(!launching && switch1_ok){
		setPort(0);
	}
	//elseif < HALF_BALLS && Switch1.ok
	//	launch = true
	else if(ballCount >= HALF_BALLS && switch1_ok){
		launching = true;
	}
	//elseif(launch)
	//	echo 1
	else if(launching){
		setPort(1);
	}
	//elseif(< MAX_BALLS && launch && switch2.ok)
	//	echo 0;
	else if(ballCount < MAX_BALLS && launching && switch2_ok){
		setPort(0);
	}
	//elseif( MAXBALLS && launch && switch.ok) 
	//	echo 1;
	//	launch = false
	else if(ballCount >= MAX_BALLS && launching && switch2_ok){
		setPort(1);
		launching = false;
	}
}

void LaunchDriver::setPort(int value){
	fs.open("/sys/class/gpio/gpio60/value"); // <<<PORT
	fs << to_string(value); // "1" for off
	fs.close();   	
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
