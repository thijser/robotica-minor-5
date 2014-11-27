#ifndef ULTRASONESENSOR_H
#define ULTRASONESENSOR_H

#include <string>

class UltrasoneSensor{
public:
	std::string tag;
	int read();
protected:
	UltrasoneSensor();
	UltrasoneSensor(std::string tag);
	std::string getTag();
	int readParse();
};

#endif
