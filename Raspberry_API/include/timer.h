/* All the timer's functions */

struct timer {
    void (*del)(const struct timer*);
};

struct timer * timer_new();