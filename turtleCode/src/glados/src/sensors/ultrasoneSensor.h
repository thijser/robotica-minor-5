#ifndef ULTRASONESENSOR_H
#define ULTRASONESENSOR_H

#include <string>

class UltrasoneSensor{
protected:
	std::string tag;
	int readParse();
public:
	UltrasoneSensor();
	UltrasoneSensor(std::string tag);
	std::string getTag();
	int read();
};

#endif
