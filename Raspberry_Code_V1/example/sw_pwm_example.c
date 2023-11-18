#include <stdio.h>
#include <bcm2835.h>
#include <pthread.h>
#include <unistd.h>
#include <wiringPi.h>

#include "main.h"
#include "include/heimdall.h"

#define RANGE 1024

// Test Thread Software PWM ?



//Define a Software PWM at 50Hz
static Sw_PWM Sw_PWM_1 = {
	.channel = 1,
	.Pin = RPI_GPIO_P1_11,
	.enable = 1,
	.range = RANGE,
	.alpha = 512,
	.nano_delay = 100e6,
	.mutex_state = PTHREAD_MUTEX_INITIALIZER,
	.mutex_data = PTHREAD_MUTEX_INITIALIZER,
    .cond_wait = PTHREAD_COND_INITIALIZER,
};

static Sw_PWM Sw_PWM_2 = {
	.channel = 2,
	.Pin = RPI_GPIO_P1_21,
	.enable = 1,
	.range = RANGE,
	.alpha = 768,
	.nano_delay = 25e6,
	.mutex_state = PTHREAD_MUTEX_INITIALIZER,
	.mutex_data = PTHREAD_MUTEX_INITIALIZER,
    .cond_wait = PTHREAD_COND_INITIALIZER,
};


static Sw_PWM Sw_PWM_3 = {
	.channel = 3,
	.Pin = RPI_GPIO_P1_15,
	.enable = 1,
	.range = RANGE,
	.alpha = 512,
	.nano_delay = 50e6,
	.mutex_state = PTHREAD_MUTEX_INITIALIZER,
	.mutex_data = PTHREAD_MUTEX_INITIALIZER,
    .cond_wait = PTHREAD_COND_INITIALIZER,
};


static Sw_PWM Sw_PWM_4 = {
	.channel = 4,
	.Pin = RPI_GPIO_P1_19,
	.enable = 1,
	.range = RANGE,
	.alpha = 256,
	.nano_delay = 75e6,
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

	// Init Software PWM
	Sw_PWM_Init(& Sw_PWM_1);
	Sw_PWM_Init(& Sw_PWM_2);
	Sw_PWM_Init(& Sw_PWM_3);
	Sw_PWM_Init(& Sw_PWM_4);

	int i = 1;
	int dir = 1;

	// Ramp up and down Duty Cycle
	while(1) {
		// Non linear behavior for fun
		Delay(((i > 100)?2:1));

		i += dir ;

		if ((i <= 0 )| (i >= RANGE)) {
			dir *= -1;
		}
		
		// Set Alpha 
		Sw_PWM_set_alpha(& Sw_PWM_1, i);
		Sw_PWM_set_alpha(& Sw_PWM_2, RANGE - i);
	}

	// Close properly the lib
	Heimdall_close();

	// Boasting ^^
	printf("Nous sommes les rois du monde ! printf \n");

    return 1;
}
