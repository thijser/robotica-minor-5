#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <string>
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
class mathasker{
public: 

ros::Subscriber sub;
ros::NodeHandle handle;
 
ros::Publisher display;
ros::Publisher sound;
ros::Publisher drive;

const int balls[20]={0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9};
std::vector<int> avaible_balls;


/* returns array of 3 integers first 2 are the once being summed third is the answer*/
int* addition2dig(){
	int* result =new int[3];
	//select indexes 
	int a;
	int b;
	do{
		a=avaible_balls[rand()%avaible_balls.size()];
		b=avaible_balls[rand()%avaible_balls.size()];
	}while(a!=b);
		result[2]=a*10+b; 
		result[1]=rand()%result[2];
		result[0]=result[2]-result[1];
	return result; 
}

int* addition1dig(){
	int* result =new int[3];
	result[2]=avaible_balls[rand()%avaible_balls.size()];
	if(result[2]!=0){
		result[1]=rand()%result[2];
	}else{
		result[1]=0;
	}
	result[0]=result[2]-result[1];
	return result; 
}

int* substract1dig(){
	int* ans=addition1dig();
	int temp=ans[0];
	ans[0]=ans[2];
	ans[2]=temp;
	return ans; 
}
int* substract2dig(){
	int* ans=addition2dig();
	int temp=ans[0];
	ans[0]=ans[2];
	ans[2]=temp;
	return ans; 		
	}

void questioncallback(std_msgs::String request){


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
	ss<<questiondata[0];
	switch (opperator){
		case '+': ss<<" en ";break;
		case '-': ss<<" min ";break;
		case 'x': ss<<" keer ";break;
		case '/': ss<<" gedeeld door ";break;
	}
	ss<<questiondata[1];
	soundmsg.data=ss.str();	
	sound.publish(soundmsg);
	std::stringstream ds;
	ds<<questiondata[0]<<opperator<<questiondata[1]<<"=";
	displaymsg.data=ds.str();
	display.publish(displaymsg);
}
mathasker():handle("~")

{
	srand (time(NULL));
	display=handle.advertise<std_msgs::String>("/display",1000);
	sound=handle.advertise<std_msgs::String>("/tawi/media/audio",1000);
	drive=handle.advertise<std_msgs::String>("/drive",1000);
	sub = handle.subscribe("/questions", 1000, &mathasker::questioncallback,this);
	for(int i=0;i<20;i++){
		avaible_balls.push_back(balls[i]);
	}
}
};
int main(int argc, char **argv){
	ros::init(argc, argv, "audioDriver");
	mathasker math;
	ros::Rate hz(100);

	while(ros::ok()){	
		ros::spinOnce();
		hz.sleep();
	} 	
}
