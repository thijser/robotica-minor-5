#include "ultrasoneSensor.h"
#include <string>

using namespace std;

UltrasoneSensor::UltrasoneSensor(){
	tag = "ultrasone";
}

UltrasoneSensor::UltrasoneSensor(std::string t){
	tag = t;
}

int UltrasoneSensor::read(){ //Just for extensiblility purposes.
	return readParse();
}

int UltrasoneSensor::readParse(){
	//TODO get an ultrasone sensor and have it work..
}