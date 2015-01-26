//publishes on /tawi/motors/wheels
//listens on /tawi/theBeat
#include <geometry_msgs/Twist.h>
#include "glados/music.h"
#include <ros/ros.h>
#include <math.h>

#define drivespeed 0.1
#define rotspeed 0000.1

class dancer{
	public:
		
	ros::Subscriber subBeat;
	ros::Subscriber subdance;
	ros::NodeHandle handle;
	ros::Publisher moveit;
	
	int dancing=1; 
	int beat=30; 
	double phase; 		//miliseconds 
	double timePerMove;	//seconds 


	int getWaittime(){
		double now =ros::Time::now().toSec();
		return timePerMove-fmod((now-phase),timePerMove);
	}
	void drivestraight(double speed){
		geometry_msgs::Twist msg;
		msg.linear.x=speed;
		msg.linear.y=0;
		msg.linear.z=0;
		msg.angular.x=0;
		msg.angular.y=0;
		msg.angular.z=0;
		moveit.publish(msg);
	}
	void rotate(double speed){	
		geometry_msgs::Twist msg;
		msg.linear.x=0;
		msg.linear.y=0;
		msg.linear.z=0;
		msg.angular.x=0;
		msg.angular.y=0;
		msg.angular.z=speed;
		moveit.publish(msg);
	}

	void backForward(){
		drivestraight(drivespeed);	
		ros::Duration(getWaittime()).sleep();
		drivestraight(-drivespeed);
		ros::Duration(getWaittime()).sleep();
	}
 
	void spinRound(){
		rotate(rotspeed);	
		ros::Duration(getWaittime()).sleep();
		rotate(-rotspeed);
		ros::Duration(getWaittime()).sleep();

	}
	void dance(){
		while(ros::ok() && dancing){
			//if(rand()%2){
				spinRound();
			//}else{
			//	backForward();
			//}
		}
	}

	void setBeat(int bpm){
		
		beat=bpm;
		timePerMove=60/(double)bpm;
	}

	void setStarttime(long time){ //miniseconds since boot 
		phase=time%(int)(timePerMove*1000);
	}

	void beatcallback(glados::music msg){
		setBeat(msg.bpm);
		setStarttime(msg.starttime);
				
	}

	dancer(): handle("~"){
		srand (time(NULL));
		moveit=handle.advertise<geometry_msgs::Twist>("/tawi/motors/drive",1000);
		subBeat = handle.subscribe("/tawi/theBeat", 1000, &dancer::beatcallback,this);
	}
};

int main(int argc, char** argv){
	ros::init(argc, argv, "teleop_turtle");
	dancer dance;
	dance.dance();
	ros::Rate hz(10);
	while(ros::ok()){
		ros::spinOnce();
		hz.sleep();
	}

}
