#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pigpio.h>


#include <sched.h>

#include "utilities.h"
#include "timing.h"

void set_realtime_priority() {
    struct sched_param param;
    param.sched_priority = 99; // Adjust priority as needed

    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        perror("sched_setscheduler");
        exit(1);
    }
}

void set_thread_affinity() {
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(3, &mask);  // Set to the desired CPU core (e.g., core 0)

    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
        perror("sched_setaffinity");
        exit(1);
    }
}

int main() {
    set_realtime_priority();
    set_thread_affinity();

    //gpioCfgClock(1, 1, 1);
    //gpioInitialise();

    struct timespec start, end, end2;

    start = gettime_nano();

    //sleep_nano(1000);
    //sleep_micros(1);

    //for(int i = 0; i<100; i++);
    //gpioDelay(1);

    end = gettime_nano();

    
    end2 = gettime_nano();

    printf("%ld:%ld to %ld:%ld\n",  start.tv_sec, start.tv_nsec, end.tv_sec, end.tv_nsec);
    printf("%ld:%ld\n", (end.tv_sec - start.tv_sec) - (end2.tv_sec - end.tv_sec), (end.tv_nsec - start.tv_nsec) - (end2.tv_nsec - end.tv_nsec));
    
    exit(0);
}
