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
	fs << 7;  							// <<< PORT
	fs.close();

	fs.open("/sys/class/gpio/gpio7/direction"); //PORT 
	fs << "in";
	fs.close();

	ros::init(argc, argv, "switchSensor");
	ros::NodeHandle handle;
	ros::Publisher pub = handle.advertise<std_msgs::Int16>("/tawi/sensors/switch", 10);
	std_msgs::Int16 msg;
	ros::Rate loop_Rate(30);

	while(ros::ok()){
		int stateRead = 2; 
		fs.open("/sys/class/gpio/gpio7/value");   // <<< PORT
		fs >> stateRead;
		fs.close();
		if(stateRead == 2)
			//ROS_INFO("stateRead not set");
		msg.data = stateRead; //Inverting for BBB logic no longer neccesary if you set port direction to in.
		
		pub.publish(msg);
		//ROS_INFO("Switch broadcasts %d", msg.data);
		loop_Rate.sleep();
	}
}
