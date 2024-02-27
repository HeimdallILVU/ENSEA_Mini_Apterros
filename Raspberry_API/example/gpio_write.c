/* Main function */
#include <bcm2835.h>
#include <stdlib.h>

#include "api.h"

int main(void) {

    struct api * api = api_new();
    struct gpio * gpio_3 = gpio_new(RPI_GPIO_P1_03); //struct gpio * gpio_3 = api->gpio_new(RPI_GPIO_P1_03);
    gpio_write(gpio_3, 0); // gpio_3->gpio_write(gpio_3, 0); 

    return 0;
}