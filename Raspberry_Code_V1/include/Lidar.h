#include <stdio.h>
#include "include/heimdall.h"
#include <stdint.h>

// Définitions des adresses et commandes du capteur LIDAR
#define SLAVE_ADDR 0x62
#define ACQ_COMMAND 0x04
#define MEASURE_DELAY 0x45
#define MAX_ACQ_COUNT 0x02
#define MEASURE_REPEAT 0x11
#define MEASURE_RESULT 0x8F
#define ACQ_CONFIG 0x04

I2C_write_to_register(uint8_t deviceAddr, uint8_t regAddr, uint8_t value);
I2C_read_from_register(uint8_t deviceAddr, uint8_t regAddr);
void lidar_init();
void lidar_start_continuous_mode();
uint16_t lidar_read_distance();