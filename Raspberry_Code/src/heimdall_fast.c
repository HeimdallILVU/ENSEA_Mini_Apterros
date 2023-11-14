#include <wiringPi.h>
#include <stdint.h>

#include "heimdall_fast.h"


void Fast_GPIO_init() {
    wiringPiSetupGpio();
}

void Fast_GPIO_set_pin_mode(uint8_t pin, uint8_t mode) {
    pinMode(pin, mode);
}

void Fast_GPIO_write(uint8_t pin, uint8_t on) {
    digitalWrite(pin, on);
}

