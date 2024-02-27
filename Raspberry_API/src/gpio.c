/* All the GPIO's functions */

#include <stdio.h>
#include <stdlib.h>

#include <bcm2835.h>

#include "gpio.h"


int gpio_read(const struct gpio* self) {
    return bcm2835_gpio_lev(self->pin);
}

void gpio_write(const struct gpio* self, int value) {
    bcm2835_gpio_write(self->pin, value);
}

void gpio_mode(const struct gpio* self, int mode) {
    bcm2835_gpio_fsel(self->pin, mode);
}

void gpio_del(const struct gpio* self) {
  free((void *)self);
}

struct gpio * gpio_new(int pin) {
    struct gpio *p = malloc(sizeof(*p));
    p->pin = pin;
    p->del = gpio_del;
    p->read = gpio_read;
    p->write = gpio_write;
    return p;
}