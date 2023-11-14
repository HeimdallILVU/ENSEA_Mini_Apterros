#include <stdio.h>
#include <bcm2835.h>

#include "main.h"
#include "include/heimdall.h"

// This code is expecting a PWM sensitive object on Pin 12 (ESC to control a turbine or an LED in series with a 220 Ohm resistor)

#define DELAY 50
#define RANGE 1024

int main() {
	
	printf("hello world !\n");

	// Init lib, return error if fail
	if (!Heimdall_init()) {
		return 1;
	}

	// Set pin 12 to PWM0
	GPIO_set_pin_mode(RPI_GPIO_P1_12, BCM2835_GPIO_FSEL_ALT5);

	// Define our pwm
	pwm_entity pwm = {0, PWM_CLOCK_DIVIDER_256, RANGE, RANGE/2, 1};
	// Init the pwm
	PWM_init(pwm);


	// Having fun with the Duty Cycle
	// Going from 0 to RANGE then back to 0
	int alpha = 1;
	int dir = 1;
	while(1) {
		if (alpha == RANGE || alpha == 0) {
			dir *= -1;
		}
		alpha += dir;
		PWM_set_alpha(pwm, alpha);
		delay(1);
	}

	// Close properly the lib
	Heimdall_close();

	// Boasting ^^
	printf("Nous sommes les rois du monde ! printf \n");

    return 1;
}
