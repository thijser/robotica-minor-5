#ifndef ULTRASONESENSOR_H
#define ULTRASONESENSOR_H

#include <string>

class UltrasoneSensor{
public:
	std::string tag;
	int readParse();
protected:
	UltrasoneSensor();
	UltrasoneSensor(std::string tag);
	std::string getTag();
	int read();
};

#endif
