#include "Lidar.h"

void lidar_init() {
    // Fixer le maximum du compteur d’acquisition
    I2C_write_to_register(SLAVE_ADDR, MAX_ACQ_COUNT, 0xFF); // définir une limite sur le nombre de mesures que le lidar va effectuer

    // Fixer la valeur du MEASURE DELAY
    I2C_write_to_register(SLAVE_ADDR, MEASURE_DELAY, 0x14); // Le temps entre deux mesures automatiques

    // Configuration du mode d’acquisition
    I2C_write_to_register(SLAVE_ADDR, ACQ_CONFIG, 0x03); // Désactivation de la correction pour plus de rapidité
}

void lidar_start_continuous_mode() {
    // Activer les mesures qui se répètent indéfinement 
    I2C_write_to_register(SLAVE_ADDR, MEASURE_REPEAT, 0xff);

    // Démarrer une mesure
    I2C_write_to_register(SLAVE_ADDR, ACQ_COMMAND, 0x04);
}
 
uint16_t lidar_read_distance() {
    // Lire la valeur mesurée
    uint16_t measurement = I2C_read_from_register(SLAVE_ADDR, MEASURE_RESULT);

    return measurement;
}