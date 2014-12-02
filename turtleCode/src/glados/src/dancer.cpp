//publishes on /tawi/motors/wheels
//listens on /tawi/theBeat
#include <geometry_msgs/Twist.h>
#include "msg/num.msg"

#define drivespeed 1;
#define rotspeed 1; 
class dancer{
	public:
		
	ros::Subscriber subBeat;
	ros::Subscriber subdance;
	ros::NodeHandle handle;
	ros::Publisher moveit;
	
	int dancing=0; 
	int beat=30; 
	double phase; 
	double timePerMove;	//seconds 
	geometry_msgs::Twist::ConstPtr &msg

	int getWaittime(){
		return timePerMove-((ros::Time::now()-phase)%timePerMove);
	}
	void drivestraight(int speed){
		msg->linear.x=speed;
		msg->linear.y=0;
		msg->linear.z=0;
		msg->angular.x=0;
		msg->angular.y=0;
		msg->angular.z=0;
	}
	void rotate(int speed){
		msg->linear.x=0;
		msg->linear.y=0;
		msg->linear.z=0;
		msg->angular.x=0;
		msg->angular.y=0;
		msg->angular.z=speed;
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
		while(dancing){
			if(rand%2){
				spinRound();
			}else{
				backForward();
			}
		}
	}

	void setBeat(int bpm){
		
		beat=bpm;
		timePerMove=60/(double)bpm
	}

	void setStarttime(long time){ //miniseconds since boot 
		phase=time%(timePerMove*1000);
	}

	void beatcallback(num.msg msg){
		setBeat(msg.beat);
		setStarttime(msg.start);
				
	}

	void 
	mathasker(): handle("~"){
		srand (time(NULL));
		moveit=handle.advertise<geometry_msgs::Twist>("/tawi/motors/wheels",1000);
		subBeat = handle.subscribe("/tawi/theBeat", 1000, &dancer::beatcallback,this);
	}
}

int main(int argc, char **argv){
	ros::init(argc, argc,"lets_dance");
	dancer dance;
	ros::rate hz(100);
	while(rosLLok()){
		ros::spinOnce();
		hz.sleep();
	}
)
}
