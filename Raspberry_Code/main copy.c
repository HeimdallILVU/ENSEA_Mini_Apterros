#include <stdio.h>
#include <bcm2835.h>
#include <pthread.h>
#include <unistd.h>

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

//struct Software_PWM
typedef struct {
	uint8_t pin; //Physical Pin
	uint8_t enable; //Enable of the output / input
	uint32_t frequency; //Frequency of the UART

	//We need to separate both of them, as toggling the Pin is not instantaneous at very high frequencies.
	pthread_t thread_sender_waiter; //The waiting thread
	pthread_t thread_sender; //The sender thread 

	pthread_t thread_reader_waiter; //The reader waiting thread
	pthread_t thread_reader; //The reader thread 

	pthread_mutex_t mutex_sender; //The mutex to protect the state value.
	pthread_mutex_t mutex_reader; //The mutex to protect the data values.
	pthread_cond_t cond_wait_sender; //The condition that the sender will wake up from.
	pthread_cond_t cond_wait_reader; //The condition that the reader will wake up from.
} UART;


int main() {
	
	printf("hello world !\n");

	// Init lib, return error if fails
	if (!Heimdall_init()) {
		return 1;
	}

	// Init Software PWM
	Sw_PWM_Init(& Sw_PWM_1);

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
	}

	// Close properly the lib
	Heimdall_close();

	// Boasting ^^
	printf("Nous sommes les rois du monde ! printf \n");

    return 1;
}
