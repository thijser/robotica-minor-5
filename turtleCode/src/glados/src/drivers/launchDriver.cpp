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
	LaunchDriver::init();
} 

LaunchDriver::LaunchDriver(SwitchSensor *s, ros::NodeHandle h){
	sensor = s;
	handle = h;
	LaunchDriver::init();
}

void LaunchDriver::init(){
	std::fstream fs;
	fs.open("/sys/class/gpio/export");
	fs << 31;  							// <<< PORT
	fs.close();

	switchSub = handle.subscribe<std_msgs::Bool>("/tawi/sensors/switch", 10, &LaunchDriver::switchCallback, this);

	launchSub = handle.subscribe<std_msgs::Int16>("/tawi/motors/launch", 10, &LaunchDriver::launchCallback, this);
	ROS_INFO("launchDriver subscribers initialised");
}

void LaunchDriver::switchCallback(const std_msgs::Bool::ConstPtr &msg){
	ROS_INFO("switchCallback");
	switchReady = msg->data;
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
	if(switchReady){
		std::fstream fs;
		//When specifing port you have to change the path names too!
		
		fs.open("/sys/class/gpio/gpio31/direction"); //PORT 
	   	fs << "out";
	   	fs.close();
	   	fs.open("/sys/class/gpio/gpio31/value"); //PORT
	  	fs << "0"; // "1" for off
	   	fs.close();
	   	ROS_INFO("Sent 0 to gpio30/value");
   	}
   	else
   		ROS_INFO("switchReady == false. Launch system still preparing");
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
	ROS_INFO("Started launchDriver");
	LaunchDriver ld;
	ld.spin();

	return 0;
}
