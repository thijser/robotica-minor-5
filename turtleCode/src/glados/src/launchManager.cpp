
#include "launchManager.h"

void LaunchManager::init(){

	coreSub = handle.subscribe<std_msgs::String>("/tawi/core/launch", 10, &LaunchManager::coreCallback, this);
	launchSub = handle.subscribe<std_msgs::Int16>("/tawi/mngr/launch", 10, &LaunchManager::launchCallback, this);
	conveySub = handle.subscribe<std_msgs::Int16>("/tawi/mngr/conveyor", 10, &LaunchManager::conveyCallback, this);

	corePub = handle.advertise<std_msgs::String>("/tawi/core/launch", 100);
	launchPub = handle.advertise<std_msgs::Int16>("/tawi/motors/launch", 100);
	conveyPub = handle.advertise<std_msgs::Int16>("/tawi/motors/conveyor", 100);
}

void LaunchManager::coreCallback(const std_msgs::String::ConstPtr &msg){
	if("startconveyor" == msg->data){
		if(!conveying){
			ROS_INFO("Launchmanager: coreCallback: pusblish 1 to conveyorDriver");
			std_msgs::Int16 message;
			message.data = 1;
			conveyPub.publish(message);
		}
		else{
			std_msgs::Int16 message;
			message.data = 0;
			conveyPub.publish(message);
		}
	}
	if("startlaunch" == msg->data){	
		if(ultrasoneSafe()){
			std_msgs::Int16 message;
			message.data = 1;
			launchPub.publish(message);
		}
		else{
			std_msgs::Int16 message;
			message.data = 0;
			launchPub.publish(message);
		}
	}
}

void LaunchManager::launchCallback(const std_msgs::Int16::ConstPtr & msg){
	if(msg->data == 1){ //currently launching
		launching = true;
	}
	if(msg->data == 0){ //currently not launching
		std_msgs::String ldonetocore;
		ldonetocore.data = "donelaunching";
		corePub.publish(ldonetocore);
		launching = false;
	}
}

void LaunchManager::conveyCallback(const std_msgs::Int16::ConstPtr & msg){
	if(msg->data == 1){ //currently working
		std_msgs::String cdonetocore;
		cdonetocore.data = "doneconveying";
		corePub.publish(cdonetocore);
	}
}

void LaunchManager::spin(){
	ros::Rate rate(10);
	ROS_INFO("Spinning LaunchManager");
	while(ros::ok()){
		ros::spinOnce();
		rate.sleep();
	}
}

bool LaunchManager::ultrasoneSafe(){
	tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
	prussdrv_init();
	/* Open PRU Interrupt */
        if (prussdrv_open (PRU_EVTOUT_0)) {
                // Handle failure
                ROS_INFO(stderr, ">> PRU open failed\n");
                return false;
        }

        /* Get the interrupt initialized */
        prussdrv_pruintc_init(&pruss_intc_initdata);

        /* Get pointers to PRU local memory */
        void *pruDataMem;
        prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &pruDataMem);
        unsigned int *pruData = (unsigned int *) pruDataMem;

        /* Execute code on PRU */
        ROS_INFO(">> Executing HCSR-04 code\n");
        prussdrv_exec_program(0, "hcsr04.bin");

        // Wait for the PRU interrupt
        prussdrv_pru_wait_event (PRU_EVTOUT_0);
        prussdrv_pru_clear_event(PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);

        // Print the distance received from the sonar
        // At 20 degrees in dry air the speed of sound is 342.2 cm/sec
        // so it takes 29.12 us to make 1 cm, i.e. 58.44 us for a roundtrip of 1 cm
        ROS_INFO("Distance = %.2f cm\n", (float) pruData[0] / 58.44);

        prussdrv_pru_disable(0);
        prussdrv_exit();
        ROS_INFO(">> PRU Disabled.\r\n");

        if(( (float) pruData[0] / 58.44) < 50){
        	return false;
        }
        else{
        	return true;
        }       

        return false;

}

int main(int argc, char **argv){
	ros::init(argc, argv, "launchManager");
	LaunchManager lm;

	lm.init();
	lm.spin();
}
