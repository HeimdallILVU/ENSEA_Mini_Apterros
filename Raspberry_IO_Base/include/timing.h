/*
    This lib is used to manage different timers.
*/


int sleep_seconds(int seconds);
int sleep_millis(int millis);
int sleep_micros(int micros);
int sleep_nano(int nano);

struct timespec gettime_nano();