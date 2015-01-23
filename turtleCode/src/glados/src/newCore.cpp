#include "newCore.h"
#include <stdlib.h>

using namespace std;
int ask =1; 
int answer=0;
void NewCore::init(){

	mngrSub = handle.subscribe<std_msgs::String>("/tawi/core/launch", 10, &NewCore::launchCallback, this);
	mngrPub = handle.advertise<std_msgs::String>("/tawi/core/launch", 100);

	mathSub = handle.subscribe<std_msgs::String>("/display", 10, &NewCore::mathCallback, this);
	serSub = handle.subscribe<std_msgs::String>("/tawi/arduino/serial", 100,&NewCore::serialCallback,this);
	mathPub = handle.advertise<std_msgs::String>("/questions", 100);

	ballPub = handle.advertise<std_msgs::Int16>("/tawi/core/ballcount", 100);
	nmbrPub = handle.advertise<std_msgs::Int16>("/tawi/core/number", 100);
}
void  speak(string say){
        std::stringstream sysCall;
        char buffer [6];
        char buffer2[12];
        say.copy(buffer,6,1);
	int k=0;
        for(int i=0;i<6;i++){
        buffer2[k]=buffer[i];
	if(buffer2[k]=='D')
		buffer2[k]='+';
	if(buffer2[k]='E')
		buffer2[k]='-';
        k++;
        sysCall<<"aplay "<<buffer<<".wav";
        buffer2[k]=' ';
        k++;
	}
	system("aplay /home/ubuntu/Pokemon.wav");

	ROS_INFO("speaking: %s", sysCall.str().c_str());
//        system(sysCall.str().c_str());

}

void NewCore::launchCallback(const std_msgs::String::ConstPtr &msg){
	if("donelaunching" == msg->data){
		stopLaunch();
	}

	if("doneconveying" == msg->data){
		startLaunch();
	}
}

void NewCore::stopLaunch(){
	ballCount = 0;
	goLaunch = false;
}

void NewCore::stopConvey(){
	//Waittime currently hardcoded in converyorDriver.cpp
	//This method is currently not used
}

void NewCore::sendLaunch(){
	if(goLaunch){
		ROS_INFO("NewCore: SendLaunch: goLaunch is true so sending startlaunch");
		std_msgs::String launchmsg;
		launchmsg.data = "startlaunch";
		mngrPub.publish(launchmsg);
	}	
}

void NewCore::startLaunch(){
	goLaunch = true;
}

void NewCore::startConvey(){
	std_msgs::String conveymsg;
	conveymsg.data = "startconveyor";
	mngrPub.publish(conveymsg);
}

void NewCore::spin(){
	ros::Rate rate(10);

	while(ros::ok()){
		NewCore::sendlaunch();
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
//	speak(shit);
	ROS_INFO("NewCore: Writing on serial through system call:%s", shit.c_str());
	std::stringstream sysCall;
        sysCall<<"/home/ubuntu/robotica-minor-5/com/arduino-serial/arduino-serial --port=/dev/ttyACM0 --send=\""<<shit<<"\""; 
	string temp= sysCall.str();
	ROS_INFO("NewCOre: syscall: %s", temp.c_str());
	system(temp.c_str());

}

void NewCore::deleteBall(const int ballnumber){ //written by bob, muchos bugs
	std_msgs::Int16 number;
	number.data = ballnumber;
	nmbrPub.publish(number);
}
int getAnsweri(int firstnumber, int operation , int secondnumber){
	ROS_INFO("NewCore: getAnsweri: %d, %d, %d", firstnumber, operation, secondnumber);
  switch(operation){
    case 'D':
      answer = firstnumber + secondnumber;
      break;
    case 'E':
      answer = firstnumber - secondnumber;
      break;
    case 'F':
      answer = firstnumber * secondnumber;
      break;
  }
  return answer;
}
int getAnswer(string s){
	return getAnsweri(s[1]-48,s[2],s[3]-48);
}

void NewCore::mathCallback(const std_msgs::String::ConstPtr& msg){
	ROS_INFO("ask= %d",ask);
	if(ask>0){
		answer=getAnswer(msg->data);
		ROS_INFO("NewCore: mathCallback: 	newSum: %s",msg->data.c_str());
		ask=0;
		writeSerial(msg->data);
	}
}

int stringToAscii(string s){
	int sum = 0;
  	for (unsigned int i = 0; i < 1; i++) {
    	sum += s[i];
  	}
  	return sum;
}

void NewCore::serialCallback(const std_msgs::String::ConstPtr& msg){
	ROS_INFO("msg-data is %d. Looking for %d", stringToAscii(msg->data), stringToAscii("c"));
	ROS_INFO("NewCore:SerialCallback: Recieved string: %s", msg->data.c_str());
	if(msg->data.find('c') != std::string::npos){
		ROS_INFO("Accepting ball and starting conveyor");
		acceptBall();
		NewCore::startConvey();
		
		ask=1;
		deleteBall(answer);
	}
	else{
		if('n' == msg->data[0]){
			
			ask=1;
			NewCore::deleteBall(answer);
		}
		ROS_INFO("NewCore: SerialCallback: message was not c");
	}
}

void NewCore::askMath(){
	ROS_INFO("asking for something");
	std_msgs::String question;
	if (rand() %2){ 
		question.data = "1digitAddition";
	}
	else{
		question.data = "1digitSubstraction";
	}
	mathPub.publish(question);
}

int main(int argc, char **argv){
	
	ros::init(argc, argv, "newCore");
	NewCore nc;
	nc.init();

	nc.spin();
}
