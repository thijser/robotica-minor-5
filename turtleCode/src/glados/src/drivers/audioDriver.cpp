/*
Uses the sound_play library of ROS to play sound on the OS' default sound driver. 
Listening on:
	/tawi/media/audio to std_msgs::String  (String to say)

Publishing on:

*/

#include <sound_play/sound_play.h>
#include <std_msgs/String.h>
#include <unistd.h>
#include <ros/ros.h>


void audioCallback(std_msgs::String msg)
{
	ros::NodeHandle publishHandle;
	sound_play::SoundClient* soundClient = new sound_play::SoundClient();
	std::cout<<msg.data<<std::endl;
	soundClient->say(msg.data);
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "audioDriver");

	ros::NodeHandle listenHandle;
	
	ros::Rate rate(1);
	sound_play::SoundClient* soundClient = new sound_play::SoundClient();
	rate.sleep();

	soundClient->playWave("$(find rrbot_description)/test.ogg"); //Yay this needs a full path name... not relative


	ros::Subscriber sub = listenHandle.subscribe("/tawi/media/audio", 1000, audioCallback);

	ros::spin();
}
