#include "newCore.h"

using namespace std;

void NewCore::init(){
	
	mngrSub = handle.subscribe<std_msgs::String>("/tawi/core/launch", 10, &NewCore::launchCallback, this);
	mngrPub = handle.advertise<std_msgs::String>("/tawi/core/launch", 100);

	mathSub = handle.subscribe<std_msgs::String>("/display", 10, &NewCore::launchCallback, this);
	mathPub = handle.advertise<std_msgs::String>("/questions", 100);

	ballPub = handle.advertise<std_msgs::Int16>("tawi/core/ballcount", 100);
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
	ros::Rate rate(100);

	while(ros::ok()){

		readSerial();

		ros::spinOnce();
		rate.sleep();
	}
}

void NewCore::acceptBall(){
	ballCount++;
	std_msgs::Int16 balls;
	balls.data = ballCount;
	ballPub.publish(balls);
}

void NewCore::readSerial(){
	//if ball accepted
	//startConvey();
	//but how..
	acceptBall();
}

void NewCore::writeSerial(string shit){
	std::fstream fs;

	fs.open("/dev/ttyO4");
	fs << shit;
	fs.close();

}

void NewCore::mathCallback(const std_msgs::String::ConstPtr &msg){
	writeSerial(msg->data);
}

void NewCore::askMath(){
	std_msgs::String question;
	question.data = "1digitAddition";
	mathPub.publish(question);
}

int main(int argc, char **argv){
	
	ros::init(argc, argv, "Core");
	NewCore nc;
	nc.init();
	nc.spin();
}
