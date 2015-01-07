#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <string.h>

ros::NodeHandle handle;
ros::Subscriber coreSub;
ros::Subscriber launchSub;
ros::Subscriber conveySub;
ros::Publisher corePub;
ros::Publisher launchPub;
ros::Publisher conveyPub;
bool launching = false;
bool conveying = false;

void init(){

	coreSub = handle.subscribe<std_msgs::String>("/tawi/core/launch", 10, &coreCallback, this);
	launchSub = handle.subscribe<std_msgs::Int16>("/tawi/mngr/launch", 10, &launchCallback, this);
	conveySub = handle.subscribe<std_msgs::Int16>("/tawi/mngr/conveyor", 10, &conveyCallback, this);

	corePub = handle.advertise<std_msgs::String>("/tawi/core/launch", 100);
	launchPub = handle.advertise<std_msgs::Int16>("/tawi/motors/launch", 100);
	conveyPub = handle.advertise<std_msgs::Int16>("/tawi/motors/conveyor", 100);
}

void coreCallback(const std_msgs::String::ConstPtr &msg){
	if(strcmp("startconveyor", msg->data) == 0){
		if(!conveying){
			std_msgs::Int16 message;
			message.data = 1;
			conveyPub.publish(message);
		}
		else{
			std_msgs::Int16 message;
			message.data = 0;
			conveyPub.publish(message);
		}
	}
	if(!launching){
		if(strcmp("startlaunch", msg->data) == 0){
			std_msgs::Int16 message;
			message.data = 1;
			launchPub.publish(message);
		}
	}
}

void launchCallback(const std_msgs::Int16::ConstPtr &msg){
	if(msg->data == 1){ //currently launching
		launching = true;
	}
	else if(launching){ //currently not launching
		std_msgs::String donetocore;
		donetocore.data = "donelaunching";
		corePub.publish(donetocore);
		launching = false;
	}
}

void conveyCallback(const std_msgs::Int16::ConstPtr &msg){

}

void spin(){
	ros::Rate rate(100);

	while(ros::ok()){
		ros::spinOnce();
		rate.sleep();
	}
}

int main(int argc, char **argv){
	ros::init(argc, argv, 'launchManager');
	init();
}