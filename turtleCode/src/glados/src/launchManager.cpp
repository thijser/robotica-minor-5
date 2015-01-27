
#include "launchManager.h"

void LaunchManager::init(){

	coreSub = handle.subscribe<std_msgs::String>("/tawi/core/launch", 10, &LaunchManager::coreCallback, this);
	launchSub = handle.subscribe<std_msgs::Int16>("/tawi/mngr/launch", 10, &LaunchManager::launchCallback, this);
	conveySub = handle.subscribe<std_msgs::Int16>("/tawi/mngr/conveyor", 10, &LaunchManager::conveyCallback, this);
	usSub = handle.subscribe<std_msgs::Float32>("/tawi/mngr/ussensor", 10, &LaunchManager::usCallback, this);

	corePub = handle.advertise<std_msgs::String>("/tawi/core/launch", 100);
	launchPub = handle.advertise<std_msgs::Int16>("/tawi/motors/launch", 100);
	conveyPub = handle.advertise<std_msgs::Int16>("/tawi/motors/conveyor", 100);
}

void LaunchManager::usCallback(const std_msgs::Float32::ConstPtr &msg){
	if(msg->data > 40){
		ultrasoneSafe = true;
	}
	else{
		ultrasoneSafe = false;
	}
}

void LaunchManager::coreCallback(const std_msgs::String::ConstPtr &msg){
	if("startconveyor" == msg->data){
		if(!conveying){
			ROS_INFO("Launchmanager: coreCallback: pusblish 1 to conveyorDriver");
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
	if("startlaunch" == msg->data){	
		if(ultrasoneSafe){
			std_msgs::Int16 message;
			message.data = 1;
			launchPub.publish(message);
		}
		else{
			std_msgs::Int16 message;
			message.data = 2;
			launchPub.publish(message);
		}
	}
}

void LaunchManager::launchCallback(const std_msgs::Int16::ConstPtr & msg){
	if(msg->data == 1){ //currently launching
		launching = true;
	}
	if(msg->data == 0){ //currently not launching
		ROS_INFO("LaunchManager: Done launching. Sending to core.");
		std_msgs::String ldonetocore;
		ldonetocore.data = "donelaunching";
		corePub.publish(ldonetocore);
		launching = false;
	}
	if(msg->data == 2){
		std_msgs::String ldonetocore;
		ldonetocore.data = "donepreparing";
		corePub.publish(ldonetocore);
		launching = false;
	}
}

void LaunchManager::conveyCallback(const std_msgs::Int16::ConstPtr & msg){
	if(msg->data == 0){ //currently working
		std_msgs::String cdonetocore;
		cdonetocore.data = "doneconveying";
		corePub.publish(cdonetocore);
	}
}

void LaunchManager::spin(){
	ros::Rate rate(10);
	ROS_INFO("Spinning LaunchManager");
	while(ros::ok()){
		ros::spinOnce();
		rate.sleep();
	}
}

int main(int argc, char **argv){
	ros::init(argc, argv, "launchManager");
	LaunchManager lm;

	lm.init();
	lm.spin();
}
