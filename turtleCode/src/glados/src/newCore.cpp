#include <ros/ros.h>
#include <std_msgs/String.h>
#include <string.h>

#define MAX_BALLS 14

void launchCallback(std_msgs::String msg);
void stopLaunch();
void stopConvey();
void startLaunch();
void startConvey();
void spin();
void readSerial();
void writeSerial();

ros::NodeHandle handle;
ros::Subscriber mngrSub;
ros::Publisher mngrPub;
int ballCount = 0;

void init(){
	mngrSub = handle.subscribe<std_msgs::String>("/tawi/core/launch", 10, launchCallback);
	mngrPub = handle.advertise<std_msgs::String>("/tawi/core/launch", 100);
}

void launchCallback(std_msgs::String msg){
	if("donelaunching" == msg.data){
		stopLaunch();
	}

	if("doneconveying" == msg.data){
		stopConvey();
	}
}

void stopLaunch(){
	ballCount = 0;
}

void stopConvey(){
	//Waittime currently hardcoded in converyorDriver.cpp
	//This method is currently not used
}

void startLaunch(){
	std_msgs::String launchmsg;
	launchmsg.data = "startlaunch";
	mngrPub.publish(launchmsg);
}

void startConvey(){
	std_msgs::String conveymsg;
	conveymsg.data = "startconveyor";
	mngrPub.publish(conveymsg);
}

void spin(){
	ros::Rate rate(100);

	while(ros::ok()){
		if(ballCount >= MAX_BALLS)
			startLaunch();

		readSerial();

		ros::spinOnce();
		rate.sleep();
	}
}

void readSerial(){
	//if ball accepted
	//startConvey();
}

void writeSerial(){
	//Something with mathasker and s123
}

int main(int argc, char **argv){
	ros::init(argc, argv, "Core");
	init();
	spin();
}
