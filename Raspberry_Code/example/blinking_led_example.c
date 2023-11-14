#include <stdio.h>
#include <bcm2835.h>

#include "main.h"
#include "include/heimdall.h"

// This code is expecting a signal sensitive object on Pin 11 (An LED in series with a 220 Ohm resistor)

#define DELAY 50

int main() {
	
	printf("hello world !\n");

	// Init lib, return error if fail
	if (!Heimdall_init()) {
		return 1;
	}

	// Set pin 11 to output
	GPIO_set_pin_mode(RPI_GPIO_P1_11, BCM2835_GPIO_FSEL_OUTP);

	// Blink
	Blink(RPI_GPIO_P1_11, DELAY, 10);

	// Close properly the lib
	Heimdall_close();

	// Boasting ^^
	printf("Nous sommes les rois du monde !\n");

    return 1;
}
