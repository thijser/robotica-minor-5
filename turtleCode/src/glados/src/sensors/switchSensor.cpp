#include "switchSensor.h"
#include <string>

using namespace std;

SwitchSensor::SwitchSensor(){
	tag = "undefined";
	state = 0;
}

SwitchSensor::SwitchSensor(string s){
	tag = s;
	state = 0;
}

//For extensibility issues
int SwitchSensor::read(){
	return getState();
}

bool SwitchSensor::setState(int s){
	state = s;
}

//Read raw information and translate it to a single int.
int SwitchSensor::readParse() {
	//TODO: Find sensor to read from...
	return 0;
}

int SwitchSensor::getState(){
	//state = readParse();
	return state;
}

string SwitchSensor::getTag(){
	return tag;
}
