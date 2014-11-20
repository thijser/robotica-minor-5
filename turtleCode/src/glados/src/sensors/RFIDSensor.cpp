#include "RFIDSensor.h"
#include <string>

using namespace std;

RFIDSensor::RFIDSensor(){
	tag = "rfid";
}

RFIDSensor::RFIDSensor(string t){
	tag = t;
}

string RFIDSensor::getTag(){
	return tag;
}

int RFIDSensor::read(){
	return readParse();
}

//translate the value scanned.
int RFIDSensor::readParse(){
	//TODO: Find RFIDSensor and its documentation
}

//Return whether there is an object to scan.
bool RFIDSensor::objectPresent(){
	//TODO: Find RFIDSensor and its documentation
}

//Scan the chip found by the reader.
int RFIDSensor::scan(){
	//TODO: Find RFIDSensor and its documentation
}