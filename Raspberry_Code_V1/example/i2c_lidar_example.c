#include <stdio.h>
#include <bcm2835.h>

#include "main.h"
#include "include/heimdall.h"

// This code is expecting a GARMIN LIDAR lite v3 at address 0x62 on SDA (Pin 3) / SCL (Pin 5)

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

	// I2c stuff
	I2C_begin();

	// I2c entity
	i2c_entity i2c1;
	i2c1.addr = 0x62;
	i2c1.baudrate = 400000;

	// Measurement loop
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
		
		// Print the value
		printf("measurement value : %u\n", val);
		// Wait a bit
		Delay(100);
		
	}

	// Close properly the lib
	Heimdall_close();

	// Boasting ^^
	printf("Nous sommes les rois du monde ! printf \n");

    return 1;
}
