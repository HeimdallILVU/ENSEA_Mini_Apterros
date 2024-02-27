/* All the GPIO's functions */

struct gpio {
    int pin;
    void (*del)(const struct gpio*);
    int (*read)(const struct gpio*);
    void (*write)(const struct gpio*, int value);
};

struct gpio * gpio_new(int pin);

int gpio_read(const struct gpio* self);
void gpio_write(const struct gpio* self, int value);
void gpio_mode(const struct gpio* self, int mode);
void gpio_del(const struct gpio* self);