#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <string>
#include <std_msgs/Int16.h>
/**
* listens to the "/questions" and "/balls" topics and sends messages to the "/makeSound", "/drive" and "/display" topic to make sounds, drive around and display various the question. 
Subscribes to:
	/questions
	/balls
	
Publishes to:
	/makeSound
	/drive
	/display	
*/ 
#define NUMBEROFBALLS 20 
class mathasker{
	public: 

	ros::Subscriber sub;
	ros::NodeHandle handle;
	 
	ros::Publisher display;
	ros::Publisher sound;
	ros::Publisher drive;

	ros::Subscriber numberball;
	ros::Subscriber launch;
	
	const int balls[NUMBEROFBALLS]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

//	const int balls[20]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19s whole CPU
//,20};
	std::vector<int> available_balls;

	//written by bob, muchos bugs but working.
	void removecallback(std_msgs::Int16 number){
		int lel = number.data;
		printf("removing number: %d \n", lel);
		//int index = find(balls, lel);
		available_balls.erase(std::remove(available_balls.begin(), available_balls.end(), lel), available_balls.end());
		/*if (index != -1){
			available_balls.erase(index);	
		}*/
		for (int i=0; i<available_balls.size(); i++){
			printf("%d, ", available_balls[i]);
		}
		printf("\n");
	}
	void printballs(){
		for(int i=0;i<available_balls.size();i++){
			ROS_INFO("ball= %d ", available_balls[i]);
		}
	}

	//written by bob, muchos bugs but working.
	void donelaunchcallback(std_msgs::String msg){

		if (msg.data == "donelaunching"){
			available_balls.clear();
			for(int i=0;i<NUMBEROFBALLS;i++){
				available_balls.push_back(balls[i]);
			}
		}
	}
	

	/* returns array of 3 integers first 2 are the once being summed third is the answer*/
	int* addition2dig(){
		int* result =new int[3];
		//select indexes 
		int a;
		int b;
		do{
			a=available_balls[rand()%available_balls.size()];
			b=available_balls[rand()%available_balls.size()];
		}while(a!=b);
			result[2]=a*10+b; 
			result[1]=rand()%result[2];
			result[0]=result[2]-result[1];
		return result; 
	}


	int* addition1dig(){
		int* result =new int[3];
		result[2]=available_balls[rand()%available_balls.size()];
		if(result[2]!=0){
			result[1]=rand()%result[2];
		}else{
			result[1]=0;
		}
		result[0]=result[2]-result[1];
		return result; 
	}

	int* substract1dig(){

		int* result =new int[3];
		result[2]=available_balls[rand()%available_balls.size()];
		ROS_INFO("question should be : %d , " , result[2] );
		if((20-result[2])==0){
			result[0]=result[2];
			result[1]=0;
		}else{
			int num = rand()%(20-result[2]);
			result[0] = num+result[2];
			ROS_INFO("num = %d" , num);
			result[1]=result[0]-result[2];
		}

		ROS_INFO("which is %d - %d" , result[0] , result[1]);
		return result; 

	}
	int* substract2dig(){
		int* ans=addition2dig();
		int temp=ans[0];
		ans[0]=ans[2];
		ans[2]=temp;
		return ans; 		
		}

	void questioncallback(std_msgs::String request){
		ROS_INFO("Mathasker: request received: %s", request.data.c_str());

		int* questiondata;
		std_msgs::String soundmsg;
		std_msgs::String displaymsg;
		char opperator='0';

		if(request.data.compare("1digitAddition")==0){

			questiondata=addition1dig();	
			opperator='+';
		}
		if(request.data.compare("2digitAddition")==0){
			questiondata=addition2dig();	
			opperator='+';
		}
		if(request.data.compare("1digitSubstraction")==0){
			questiondata=substract1dig();	
			opperator='-';
		}
		if(request.data.compare("2digitSubstraction")==0){
			questiondata=substract2dig();	
			opperator='-';
		}
		if (opperator=='0'){
			std::cout<<"/question:"<<request<<"is not understanable for the mathasker"<<std::endl;
			return;
		}

		std::stringstream ss;
		char dig = (char)(((int)'0')+questiondata[0]);;
		dig=dig+questiondata[0];
		ss<<dig;
		switch (opperator){
			case '+': ss<<"D";break;
			case '-': ss<<"E";break;
			case 'x': ss<<"F";break;
			case '/': ss<<"G";break;
		}
		dig = (char)(((int)'0')+questiondata[1]);
		ss<<dig;
		soundmsg.data=ss.str();	
		sound.publish(soundmsg);
		std::stringstream ds;
		 dig = (char)(((int)'0')+questiondata[0]);;

		ds<<"s"<<dig;
		switch (opperator){
			case '+': ds<<'D';break;
			case '-': ds<<"E";break;
			case 'x': ds<<"F";break;
			case '/': ds<<"G";break;
		}
		 dig = (char)(((int)'0')+questiondata[1]);;
		ds<<dig;
		std::cout<<ds<<std::endl;
		displaymsg.data=ds.str();
		ROS_INFO("publishing /display msg");
		display.publish(displaymsg);
	//	ROS_INFO("sending: %S" , ds.str().c_str());
	}


	mathasker():handle("~"){
		srand (time(NULL));
		display=handle.advertise<std_msgs::String>("/display",100);
		sound=handle.advertise<std_msgs::String>("/tawi/media/audio",100);
		drive=handle.advertise<std_msgs::String>("/drive",100);
		sub = handle.subscribe<std_msgs::String>("/questions", 100, &mathasker::questioncallback,this); //question TRIGGER
		numberball = handle.subscribe<std_msgs::Int16>("/tawi/core/number", 1, &mathasker::removecallback,this); //remove ball subscription
		launch = handle.subscribe<std_msgs::String>("/tawi/core/launch", 100, &mathasker::donelaunchcallback,this); //done with launching subscription
		for(int i=1;i<20;i++){
			available_balls.push_back(balls[i]);
		}
	}
};


int main(int argc, char **argv){
	ros::init(argc, argv, "audioDriver");
	mathasker math;
	ros::Rate hz(10);

	while(ros::ok()){	

		ros::spinOnce();
		hz.sleep();
	} 	
}
