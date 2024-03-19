#include <stdio.h>
#include <stdint.h>

// Définitions des adresses et commandes du capteur LIDAR
#define SLAVE_ADDR 0x62
#define SLAVE_I2C_SPEED 400000
#define ACQ_COMMAND 0x04
#define MEASURE_DELAY 0x45
#define MAX_ACQ_COUNT 0x02
#define MEASURE_REPEAT 0x11
#define MEASURE_RESULT 0x8F
#define ACQ_CONFIG 0x04
#define PENDING_MEASUREMENT 0x01
#define MSB_MEASURE 0x0f
#define LSB_MEASURE 0x10
#define START_REGISTER 0x00
#define RUN_MEASUREMENT_COMMAND 0x04 

typedef struct  {
    uint8_t addr;
    uint32_t baudrate;
} lidar_entity;

void lidar_init(lidar_entity);
void lidar_start_continuous_mode(lidar_entity);
uint16_t lidar_read_distance(lidar_entity);