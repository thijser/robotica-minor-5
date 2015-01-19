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
	int validatemsg(std::string msg){
		if(msg[0]=='c'||msg[0]=='w')
			return 1;
		return 0;
	}
	void writeSerial(std::string shit){
		ROS_INFO("confirming");
		std::stringstream sysCall;
	        sysCall<<"/home/ubuntu/robotica-minor-5/com/arduino-serial/arduino-serial --port=/dev/ttyACM0 --send="<<shit; 	
		std::string temp= sysCall.str();
		system(temp.c_str());
	}

	std::stringstream read;

	void poll(){
		ROS_INFO("polling");
		read<<(char*)readline();

		if(read.str()[0]!=0&&read.str()[0]!=10){
			serialmsg.data=read.str();
			ROS_INFO("SerialListener:received data: %s" , read.str().c_str());
			if(checklastln(read.str().c_str(),255)){
				if(validatemsg(read.str())){	
					writeSerial("conf");				
					arduino.publish(serialmsg);
					ROS_INFO("SerialListener: Read %s", read.str().c_str());
					read.str("");	
				}
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
