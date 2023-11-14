#include <stdio.h>
#include <bcm2835.h>

#include "main.h"
#include "include/heimdall.h"

// This code is expecting a GARMIN LIDAR lite v3 at address 0x62 on SDA (Pin 3) / SCL (Pin 5)
// This code is expecting a PWM sensitive object on Pin 12 (ESC to control a turbine or an LED in series with a 220 Ohm resistor)

#define DELAY 50
#define RANGE 128

#define MSB_MEASURE 0x0f
#define LSB_MEASURE 0x10

#define START_REGISTER 0x00
#define PENDING_MEASUREMENT 0x01

#define RUN_MEASUREMENT_COMMAND 0x04 

int main() {
	
	printf("hello world !\n");

	// Init lib, return error if fail
	if (!Heimdall_init()) {
		return 1;
	}

	// Set pin 12 to PWM0
	GPIO_set_pin_mode(RPI_GPIO_P1_12, BCM2835_GPIO_FSEL_ALT5);

	// Define our pwm
	pwm_entity pwm = {0, PWM_CLOCK_DIVIDER_64, RANGE, 0, 1};
	// Init the pwm
	PWM_init(pwm);

	// I2c stuff
	I2C_begin();

	// I2c entity
	i2c_entity i2c1;
	i2c1.addr = 0x62;
	i2c1.baudrate = 400000;


	// Having fun with the Duty Cycle using the lidar values
	int alpha = 0;
	int i = 0;

	while(1) {
		// Begin a measure
		I2C_write_to_register(i2c1, START_REGISTER, RUN_MEASUREMENT_COMMAND);

		// wait for the end of measure
		while((I2C_read_from_register(i2c1, PENDING_MEASUREMENT) & 1) != 0 ) {
			Delay(1);
		}

		// Read two register MSB and LSB of measurement value
		char val_msb = I2C_read_from_register(i2c1, MSB_MEASURE);
		char val_lsb = I2C_read_from_register(i2c1, LSB_MEASURE);
		
		// Make the 16 bits value from the two 8 bits MSB and LSB
		uint16_t val = ((uint16_t) val_msb << 8) | val_lsb;

		// Define alpha from the measurement val
		alpha = (RANGE - val >= 0)?RANGE - val:0;

		// Set the alpha value from our alpha value
		PWM_set_alpha(pwm, alpha);

		// Print Stuff
		i++;
		if (i%25 == 0) printf("alpha : %u, measurement : %u\n", alpha, val);
		Delay(1);
	}

	// Close properly the lib
	Heimdall_close();

	// Boasting ^^
	printf("Nous sommes les rois du monde ! printf \n");

    return 1;
}
