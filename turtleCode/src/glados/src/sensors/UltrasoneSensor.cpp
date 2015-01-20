#include "UltrasoneSensor.h"

int UltrasoneSensor::init(void) {

	pub = handle.advertise<std_msgs::Float32>("/tawi/mngr/ussensor", 100);
	/* Initialize the PRU */
	printf(">> Initializing PRU\n");
	tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
	prussdrv_init();

	/* Open PRU Interrupt */
	if (prussdrv_open (PRU_EVTOUT_0)) {
		// Handle failure
		fprintf(stderr, ">> PRU open failed\n");
		return 1;
	}

	/* Get the interrupt initialized */
	prussdrv_pruintc_init(&pruss_intc_initdata);

	/* Get pointers to PRU local memory */
	void *pruDataMem;
	prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &pruDataMem);
	unsigned int *pruData = (unsigned int *) pruDataMem;

	/* Execute code on PRU */
	printf(">> Executing HCSR-04 code\n");
	prussdrv_exec_program(0, "hcsr04.bin");
	
	return (0);

}

void UltrasoneSensor::spin(){
	ros::Rate rate(1);
	while (ros::ok()) {
		
		// Wait for the PRU interrupt
		prussdrv_pru_wait_event (PRU_EVTOUT_0);
		prussdrv_pru_clear_event(PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
		
		// Print the distance received from the sonar
		// At 20 degrees in dry air the speed of sound is 342.2 cm/sec
		// so it takes 29.12 us to make 1 cm, i.e. 58.44 us for a roundtrip of 1 cm
		printf("%3d: Distance = %.2f cm\n", i, (float) pruData[0] / 58.44);

		std_msgs::Float32 msg;
		msg.data = (float) pruData[0] / 58.44);
		corePub.publish(msg);

		rate.sleep();
	}
}

int main(int argc, char **argv){
	ros::init(argc, argv, "ultraSone");
	UltrasoneSensor us;
	us.init();
	us.spin();

	prussdrv_pru_disable(0);
	prussdrv_exit();
	printf(">> PRU Disabled.\r\n");
}