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

LaunchDriver::LaunchDriver () {
	sensor = new SwitchSensor("Rack");
	ros::NodeHandle nh;
	handle = nh;
} 

LaunchDriver::LaunchDriver(SwitchSensor *s, ros::NodeHandle h){
	sensor = s;
	handle = h;
}

void LaunchDriver::init(){
	switchSub = handle.subscribe<std_msgs::Bool>("/tawi/motors/launch", 10, &LaunchDriver::switchCallback, this);

	launchSub = handle.subscribe<std_msgs::Int16>("/tawi/sensors/switch", 10, &LaunchDriver::launchCallback, this);
}

void LaunchDriver::switchCallback(const std_msgs::Bool::ConstPtr &msg){
	switchReady = msg->data;
}

void LaunchDriver::launchCallback(const std_msgs::Int16::ConstPtr &msg){
	if(msg->data == 1){
		launch();
	}
}

bool LaunchDriver::launch(){
	std::fstream fs;
	//When specifing port you have to change the path names too!
	fs.open("/sys/class/gpio/export");
	fs << 30;  //PORT
	fs.close();
	fs.open("/sys/class/gpio/gpio30/direction"); //PORT 
   	fs << "out";
   	fs.close();
   	fs.open("/sys/class/gpio/gpio30/value"); //PORT
  	fs << "0"; // "1" for off
   	fs.close();
}

void LaunchDriver::spin() {
	ROS_INFO("Spinning launchDriver");

	ros::Rate r(1000);

	while(ros::ok()) {

		ros::spinOnce();
		r.sleep();
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "launchDriver");
	LaunchDriver ld;
	ld.spin();

	return 0;
}