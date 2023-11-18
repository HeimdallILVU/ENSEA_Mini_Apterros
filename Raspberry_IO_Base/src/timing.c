#define _POSIX_C_SOURCE 200112L

#include <time.h>
#include <stdio.h>
#include <bcm2835.h>


struct timespec sleepTime;

int sleep_seconds(int seconds) {
    return seconds;
}

int sleep_millis(int millis) {
    return millis;
}

int sleep_micros(int micros) {
    delayMicroseconds(micros);
    return 0;
}

int sleep_nano(int nano) {
    sleepTime.tv_sec = nano / 1e9;
    sleepTime.tv_nsec = nano % ((int) 1e9);

    if (clock_nanosleep(CLOCK_REALTIME, 0, &sleepTime, NULL) != 0) {
        perror("clock_nanosleep");
        return 1;
    }

    return 0;
}


struct timespec gettime_nano() {

    struct timespec gettime_now;
    clock_gettime(CLOCK_REALTIME, &gettime_now);

    return gettime_now;
}