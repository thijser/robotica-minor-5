#ifndef SWITCHSENSOR_H
#define SWITCHSENSOR_H

#include <string>

class SwitchSensor{
public:
	SwitchSensor();
	SwitchSensor(std::string tag);
	int read();
	bool setState(int s);
protected:

	int state;
	std::string tag;
	int getState();
	int readParse();
};

#endif