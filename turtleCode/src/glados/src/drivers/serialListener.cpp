#include <ros/ros.h>
#include <std_msgs/String.h>
#include <string>
#include "serialDriver.c"
#include <sstream>      // std::stringstream

class serialListener{
	public:
	ros::NodeHandle handle;
	ros::Publisher arduino;
	std_msgs::String serialmsg;
	int checklastln(const char* toCheck , int maxLength){
		for(int i=0;i<maxLength;i++){
			if(toCheck[i]==10){
				return 1;
			}
			if(toCheck[i]==0){
				return 0;
			}
		}
	}
	std::stringstream read;
	void poll(){
		ROS_INFO("polling");
		read<<(char*)readline();

		if(read.str()[0]!=0&&read.str()[0]!=10){
			serialmsg.data=read.str();
			if(checklastln(read.str().c_str(),255)){
				arduino.publish(serialmsg);
				ROS_INFO("SerialListener: Read %s", read.str());
				read.str("");	
			}
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
