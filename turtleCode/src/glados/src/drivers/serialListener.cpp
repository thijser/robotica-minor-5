#include <ros/ros.h>
#include <std_msgs/String.h>
#include <string>
#include "serialDriver.c"

class serialListener{
	public:
	ros::NodeHandle handle;
	ros::Publisher arduino;
	std_msgs::String serialmsg;
	void poll(){
		char* read=(char*)readline();
		if(read[0]!=0&&read[0]!=10){
			serialmsg.data=read;
			arduino.publish(serialmsg);
		}
	}

	serialListener():handle("~"){
		arduino=handle.advertise<std_msgs::String>("/tawi/arduino/serial",100);
	}
};


int main (int argc, char **argv){
	ros::init(argc, argv, "serialListener");
	serialListener serial;
	ros::Rate hz(5);
	while(ros::ok()){
		serial.poll();
		ros::spinOnce();	
		hz.sleep();
	}
}
