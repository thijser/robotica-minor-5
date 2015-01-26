#include <string>
#include <iostream>
#include <ros/ros.h>

void sayc(const char c, int pos){

	std::stringstream sysCall;
	sysCall<<"aplay ";
	sysCall<<"/home/ubuntu/tawisound/";
	if(c=='D'){
		sysCall<<"plus.wav";
		system(sysCall.str().c_str());
		return;
	}
	if(c=='E'){
		sysCall<<"min.wav";
		system(sysCall.str().c_str());
		return;
	}
	if(c=='F'){
		sysCall<<"keer.wav";
		system(sysCall.str().c_str());
		return;
	}
	if(pos==1){
		sysCall<<"f";
	}else{
		sysCall<<"L";
	}
 	const char* what=std::to_string(c-48).c_str(); 
	sysCall<<what<<".wav";
	ROS_INFO("speak: %s", sysCall.str().c_str());
	system(sysCall.str().c_str());
}

void opening(){
        std::stringstream sysCall;
	sysCall<<"aplay /home/ubuntu/tawisound/wat_is.wav";
	system(sysCall.str().c_str());
}

void speak(const char* say){
 	opening();
	for(int i=1;i<4;i++){
		sayc(say[i],i);
	}
}
