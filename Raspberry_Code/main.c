#define _POSIX_C_SOURCE 200112L


#include <stdio.h>
#include <bcm2835.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "main.h"
#include "include/heimdall.h"

#define RANGE 128
#define SCALE 1e3

// Test Thread Software PWM ?



//Define a Software PWM at 50Hz
static Sw_PWM Sw_PWM_1 = {
	.channel = 1,
	.Pin = RPI_GPIO_P1_11,
	.enable = 1,
	.range = RANGE,
	.alpha = 32,
	.frequency = 1 * SCALE,
	.mutex_state = PTHREAD_MUTEX_INITIALIZER,
	.mutex_data = PTHREAD_MUTEX_INITIALIZER,
    .cond_wait = PTHREAD_COND_INITIALIZER,
};

static Sw_PWM Sw_PWM_2 = {
	.channel = 2,
	.Pin = RPI_GPIO_P1_21,
	.enable = 1,
	.range = RANGE,
	.alpha = 8,
	.frequency = 2 * SCALE,
	.mutex_state = PTHREAD_MUTEX_INITIALIZER,
	.mutex_data = PTHREAD_MUTEX_INITIALIZER,
    .cond_wait = PTHREAD_COND_INITIALIZER,
};





int main() {
	
	printf("hello world !\n");

	// Init lib, return error if fails
	if (!Heimdall_init()) {
		return 1;
	}

	// Set pin 12 to PWM0
	GPIO_set_pin_mode(RPI_GPIO_P1_12, BCM2835_GPIO_FSEL_ALT5);

	// Define our pwm
	pwm_entity pwm = {0, PWM_CLOCK_DIVIDER_1, 2 * RANGE, (2*RANGE)/10 + 8, 1};
	// Init the pwm
	PWM_init(pwm);

	// Init Software PWM
	Sw_PWM_Init(& Sw_PWM_1);
	Sw_PWM_Init(& Sw_PWM_2);

	//Sw_PWM_join(& Sw_PWM_1);

	int i = 1;
	int dir = 1;

	// Ramp up and down Duty Cycle
	while(1) {
		// Non linear behavior for fun
		Delay(10);

		i += dir ;

		if ((i <= 0 ) | (i >= RANGE)) {
			dir *= -1;
		}
		
		// Set Alpha 
		Sw_PWM_set_alpha(& Sw_PWM_1, i);
	 	Sw_PWM_set_alpha(& Sw_PWM_2, i);
		PWM_set_alpha(pwm, i);
	}

	// Close properly the lib
	Heimdall_close();

	// Boasting ^^
	printf("Nous sommes les rois du monde ! printf \n");

    return 1;
}
