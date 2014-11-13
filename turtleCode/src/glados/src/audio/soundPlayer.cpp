/*
Uses the sound_play library of ROS to play sound on the OS' default sound driver. 
Listening on:
	/tawi/media/audio to std_msgs::String  (String to say)

Publishing on:

*/

#include <sound_play/sound_play.h>
#include <std_msgs/String.h>
#include <unistd.h>

void audioCallback(std_msgs::String msg)
{
	ros:NodeHandle publishHandle;
	sound_play::SoundClient soundClient;

	sc.say("Testing");
}


int main(int argc, char **argv)
{
	ros::init(Argc, argv, "soundPlayer");

	ros::NodeHandle listenHandle;
	

	ros::Subscriber sub = listenHandle.subscribe("/tawi/media/audio", 1000, audioCallback);

	ros::spin();
}