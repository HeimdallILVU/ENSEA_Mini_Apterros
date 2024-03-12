/* All the API's functions */

#include <stdlib.h>

#include "api.h"

void api_del(const struct api* self) {
  free((void *)self);
}


struct api * api_new() {
    struct api *p = malloc(sizeof(*p));
    p->del = api_del;
    p->gpio_new = gpio_new;
    p->timer_new = timer_new;
    p->parallel_new = parallel_new;
    return p;
}

