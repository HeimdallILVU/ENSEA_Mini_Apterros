/* All the API's functions */

#include "gpio.h"
#include "parallel.h"
#include "timer.h"

struct api {
    void (*del)(const struct api*);
    struct gpio* (*gpio_new)(int);
    struct timer* (*timer_new)(int);
    struct parallel* (*parallel_new)(int);
};

struct api * api_new();
