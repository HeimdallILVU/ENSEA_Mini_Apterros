#include "Lidar.h"
#include "heimdall.h"

void lidar_init(lidar_entity lidar_entity) {


    // I2c stuff
    I2C_begin();

    lidar_entity.addr = SLAVE_ADDR;
    lidar_entity.baudrate = SLAVE_I2C_SPEED;

    i2c_entity i2c_entity;
    i2c_entity.addr = lidar_entity.addr;
    i2c_entity.baudrate = lidar_entity.baudrate;

    // Fixer le maximum du compteur d’acquisition
    I2C_write_to_register(i2c_entity, MAX_ACQ_COUNT, 0xFF); // définir une limite sur le nombre de mesures que le lidar va effectuer

    // Fixer la valeur du MEASURE DELAY
    I2C_write_to_register(i2c_entity, MEASURE_DELAY, 0x14); // Le temps entre deux mesures automatiques

    // Configuration du mode d’acquisition
    I2C_write_to_register(i2c_entity, ACQ_CONFIG, 0x03); // Désactivation de la correction pour plus de rapidité
}

void lidar_start_continuous_mode(lidar_entity lidar_entity) {
    i2c_entity i2c_entity;
    i2c_entity.addr = lidar_entity.addr;
    i2c_entity.baudrate = lidar_entity.baudrate;
    // Activer les mesures qui se répètent indéfinement 
    //I2C_write_to_register(i2c_entity, MEASURE_REPEAT, 0xff);
    //I2C_write_to_register(i2c_entity, MEASURE_DELAY, 0x14);

    // Démarrer une mesure
    //I2C_write_to_register(i2c_entity, ACQ_COMMAND, 0x04);

    I2C_write_to_register(i2c_entity, START_REGISTER, RUN_MEASUREMENT_COMMAND);

}
 
uint16_t lidar_read_distance(lidar_entity lidar_entity) {

    i2c_entity i2c_entity;
    i2c_entity.addr = lidar_entity.addr;
    i2c_entity.baudrate = lidar_entity.baudrate;

    I2C_write_to_register(i2c_entity, START_REGISTER, RUN_MEASUREMENT_COMMAND);

    
    // wait for the end of measure
    while ((I2C_read_from_register(i2c_entity, PENDING_MEASUREMENT) & 1) != 0) {
        Delay(1);
    }
    

    // Read two register MSB and LSB of measurement value
    char val_msb = I2C_read_from_register(i2c_entity, MSB_MEASURE);
    char val_lsb = I2C_read_from_register(i2c_entity, LSB_MEASURE);

    // Make the 16 bits value from the two 8 bits MSB and LSB
    uint16_t measurement = ((uint16_t)val_msb << 8) | val_lsb;

    return measurement;
}