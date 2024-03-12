/* All the parallel's functions */

#include <stdlib.h>

#include "parallel.h"

void parallel_del(const struct parallel* self) {
  free((void *)self);
}

struct parallel * parallel_new() {
    struct parallel *p = malloc(sizeof(*p));
    p->del = parallel_del;
    return p;
}