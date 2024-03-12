/* All the timer's functions */

#include <stdlib.h>

#include "timer.h"

void timer_del(const struct timer* self) {
  free((void *)self);
}

struct timer * timer_new() {
    struct timer *p = malloc(sizeof(*p));
    p->del = timer_del;
    return p;
}