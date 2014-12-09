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
//ahahaha deze code. Volgens mij gaat nu geen een van de bovenstaande functies werken. 
//Dit moet nog naar de SwitchSensor namespace verplaatst worden
int main(int argc, char **argv) {
	std::fstream fs;
	fs.open("/sys/class/gpio/export");
	fs << 60;  							// <<< PORT
	fs.close();

	fs.open("/sys/class/gpio/gpio30/direction"); //PORT 
	fs << "in";
	fs.close();

	ros::init(argc, argv, "switchSensor");
	ros::NodeHandle handle;
	ros::Publisher pub = handle.advertise<std_msgs::Int16>("/tawi/sensors/switch", 100);
	std_msgs::Int16 msg;
	ros::Rate loop_Rate(10);

	while(ros::ok()){
		int stateRead = 0; 
		fs.open("/sys/class/gpio/gpio60/value");   // <<< PORT
		fs >> stateRead;
		fs.close();
		msg.data = 1-stateRead; //Inverting for BBB logic

		pub.publish(msg);

		loop_Rate.sleep();
	}
}