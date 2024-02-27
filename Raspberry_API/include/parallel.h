/* All the parallel's functions */

struct parallel {
    void (*del)(const struct parallel*);
};

struct parallel * parallel_new();