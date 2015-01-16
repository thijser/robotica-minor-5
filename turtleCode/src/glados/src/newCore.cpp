#include "newCore.h"

using namespace std;
int ask =1; 
void NewCore::init(){
	
	mngrSub = handle.subscribe<std_msgs::String>("/tawi/core/launch", 10, &NewCore::launchCallback, this);
	mngrPub = handle.advertise<std_msgs::String>("/tawi/core/launch", 100);

	mathSub = handle.subscribe<std_msgs::String>("/display", 10, &NewCore::mathCallback, this);
	serSub = handle.subscribe<std_msgs::String>("/tawi/arduino/serial", 100,&NewCore::serialCallback,this);
	mathPub = handle.advertise<std_msgs::String>("/questions", 100);

	ballPub = handle.advertise<std_msgs::Int16>("/tawi/core/ballcount", 100);
	nmbrPub = handle.advertise<std_msgs::Int16>("/tawi/core/number", 100);
}

void NewCore::launchCallback(const std_msgs::String::ConstPtr &msg){
	if("donelaunching" == msg->data){
		stopLaunch();
	}

	if("doneconveying" == msg->data){
		stopConvey();
	}
}

void NewCore::stopLaunch(){
	ballCount = 0;
}

void NewCore::stopConvey(){
	//Waittime currently hardcoded in converyorDriver.cpp
	//This method is currently not used
}

void NewCore::startLaunch(){
	std_msgs::String launchmsg;
	launchmsg.data = "startlaunch";
	mngrPub.publish(launchmsg);
}

void NewCore::startConvey(){
	std_msgs::String conveymsg;
	conveymsg.data = "startconveyor";
	mngrPub.publish(conveymsg);
}

void NewCore::spin(){
	ros::Rate rate(10);

	while(ros::ok()){

		if (ask){			
			askMath();}
		ros::spinOnce();
		rate.sleep();
	}
}

void NewCore::acceptBall(){
	ROS_INFO("Ball accepted. Publishing to LaunchManager");
	ballCount++;
	std_msgs::Int16 balls;
	balls.data = ballCount;
	ballPub.publish(balls);
}

void NewCore::writeSerial(string shit){
	ROS_INFO("writing shit");
	std::stringstream sysCall;
        sysCall<<"/home/ubuntu/robotica-minor-5/com/arduino-serial/arduino-serial --port=/dev/ttyACM0 --send="<<shit; 
	string temp= sysCall.str();
	system(temp.c_str());

}

void NewCore::deleteBall(const int ballnumber){ //written by bob, muchos bugs
	std_msgs::Int16 number;
	number.data = ballnumber;
	nmbrPub.publish(number);
}

void NewCore::mathCallback(const std_msgs::String::ConstPtr& msg){
	ROS_INFO("ask= %d",ask);
	if(ask>0){
		printf("newSum%s",msg->data.c_str());
		ask--;
		writeSerial(msg->data);
	}
}

void NewCore::serialCallback(const std_msgs::String::ConstPtr& msg){
	const char* data = msg->data.c_str();
	if(strcmp(data,"cor")==0){
		acceptBall();
		NewCore::startConvey();
		ask++;
	}
}
void NewCore::askMath(){
	ROS_INFO("asking for something");
	std_msgs::String question;
	question.data = "1digitAddition";
	mathPub.publish(question);
}

int main(int argc, char **argv){
	
	ros::init(argc, argv, "newCore");
	NewCore nc;
	nc.init();

	nc.spin();
}
