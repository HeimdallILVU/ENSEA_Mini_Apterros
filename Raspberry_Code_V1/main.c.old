#define _POSIX_C_SOURCE 200112L


#include <stdio.h>
#include <bcm2835.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "main.h"
#include "include/heimdall.h"


int main() {
	
	printf("hello world !\n");

	// Init lib, return error if fails
	if (!Heimdall_init()) {
		return 1;
	}

	// Set pin 12 to PWM0
	GPIO_set_pin_mode(RPI_GPIO_P1_11, BCM2835_GPIO_FSEL_OUTP);

	char buf[5];

	buf[0] = 0x82;
	buf[1] = 0xAA;
	buf[2] = 0x55;
	buf[3] = 0xA5;
	buf[4] = 0x5A;
	
	while(1) {
		UART_Write(RPI_GPIO_P1_11, 1e6, buf);
		Delay(100);
	}

	// Close properly the lib
	Heimdall_close();

	// Boasting ^^
	printf("Nous sommes les rois du monde ! printf \n");

    return 1;
}
