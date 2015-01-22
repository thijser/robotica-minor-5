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
	std::stringstream read;

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

	/*Now checks whether string contains the character, rather than if it is located at the first position.
	More sensitive to anomalies, but so be it. */
	int validatemsg(std::string msg){
		ROS_INFO("SerialListener: validatemsg: %s", msg.c_str());
		//if(msg[0]=='c'||msg[0]=='w'){
		if(msg.find('c') != std::string::npos || msg.find('w') != std::string::npos|| msg.find('n') != std::string::npos){
			return 1;
		}
		ROS_INFO("SerialListener: Invalid message received");
		return 0;
	}

	void writeSerial(std::string shit){
		ROS_INFO("SerialListener: Writing to serial line");
		std::stringstream sysCall;
	        sysCall<<"/home/ubuntu/robotica-minor-5/com/arduino-serial/arduino-serial --port=/dev/ttyACM0 --send="<<shit; 	
		std::string temp= sysCall.str();
		ROS_INFO("serialListener: syscall: %s", temp.c_str());
		system(temp.c_str());
	}

	void poll(){
		ROS_INFO("SerialListener: Ping before readline");
		read<<(char*)readline(); // what are you reading? This is locking the program
		ROS_INFO("SerialListener: poll: Read some line");
	//	if(read.str()[0]!=0&&read.str()[0]!=10){ //what is even going on here
			serialmsg.data=read.str();
			ROS_INFO("SerialListener:received data: %s" , read.str().c_str());
			if(checklastln(read.str().c_str(),255)){
				if(validatemsg(read.str())){	
					writeSerial("conf");				
					arduino.publish(serialmsg);
					read.str("");	
				
				}
			}
	//	}
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

