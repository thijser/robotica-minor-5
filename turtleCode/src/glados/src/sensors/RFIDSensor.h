#ifndef RFIDSENSOR_H
#define RFIDSENSOR_H

#include <string>

class RFIDSensor{
public:
	RFIDSensor();
	RFIDSensor(std::string);
	std::string getTag();
	int read();
protected:
	int readParse();
	int scan();
	std::string tag;
	bool objectPresent();
};

#endif