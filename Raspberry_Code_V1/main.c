#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <bcm2835.h>

#include "main.h"
#include "include/heimdall.h"
#include "include/Lidar.h"

#define DELAY 10
#define RANGE 4096

#define MAX_REVERSE_RANGE 923
#define STOP_RANGE 1259
#define MAX_FORWARD_RANGE 1594
#define MAX_DISTANCE 50
#define AVERAGER_NUMBER_OF_MEASUREMENT 25

#define MSB_MEASURE 0x0f
#define LSB_MEASURE 0x10
#define START_REGISTER 0x00

#define RUN_MEASUREMENT_COMMAND 0x04

void child_process(int pipe_write_fd) {

    // Lidar entity
    lidar_entity lidar_entity;

    // Making a list of recent readings
    int sweeper = 0;
    int values[AVERAGER_NUMBER_OF_MEASUREMENT] = {0};
    int i = 0;


    lidar_init(lidar_entity);

    lidar_start_continuous_mode(lidar_entity);

    printf("child starting while(1)\n");

    while (1) {

        uint16_t val = lidar_read_distance(lidar_entity);

        // Store the reading
        values[sweeper] = val;
        sweeper = (sweeper >= AVERAGER_NUMBER_OF_MEASUREMENT - 1) ? 0 : sweeper + 1;

        // Calculate average
        int tmp = 0;
        for (int j = 0; j < AVERAGER_NUMBER_OF_MEASUREMENT; j++) {
            tmp += values[j];
        }
        val = tmp / AVERAGER_NUMBER_OF_MEASUREMENT;

        // Write the value to the pipe
        write(pipe_write_fd, &val, sizeof(uint16_t));

        i++;
        if (i%100 == 0) printf("measurement_child : %u\n", val);
        // Delay to maintain frequency
        //Delay(1000 / 400); // 400Hz
    }
}



void parent_process(int pipe_read_fd) {
    // Set pin 12 to PWM0
    GPIO_set_pin_mode(RPI_GPIO_P1_12, BCM2835_GPIO_FSEL_ALT5);

    // Define our pwm
    pwm_entity pwm = {0, PWM_CLOCK_DIVIDER_64, RANGE, 1259, 1};
    // Init the pwm
    PWM_init(pwm);

    uint16_t val;
    uint16_t latest_val = 0;
    int i = 0;

    
    // Set pipe to non-blocking mode
    int flags = fcntl(pipe_read_fd, F_GETFL, 0);
    fcntl(pipe_read_fd, F_SETFL, flags | O_NONBLOCK);

    while (1) {
        // Read all available values from the pipe and keep only the last one
        ssize_t bytes_read;
        do {
            bytes_read = read(pipe_read_fd, &val, sizeof(uint16_t));
            if (bytes_read == sizeof(uint16_t))
                latest_val = val;
        } while (bytes_read == sizeof(uint16_t));

        // Use the latest value for PWM control
        int alpha = ((latest_val >= MAX_DISTANCE) || (latest_val < 0)) ? STOP_RANGE : ((MAX_DISTANCE - latest_val) * (MAX_REVERSE_RANGE - STOP_RANGE)) / MAX_DISTANCE + STOP_RANGE;
        PWM_set_alpha(pwm, alpha);

        i++;
        if (i%100 == 0) printf("alpha : %u, measurement_father : %u\n", alpha, val);

        Delay(1000 / 100); // 100Hz
    }
}


int main() {
    printf("hello world !\n");

    // Init lib, return error if fail
	if (!Heimdall_init()) {
		return 1;
	}

    // Create a pipe for communication between processes
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return 1;
    }

    // Fork a child process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Child process
        close(pipe_fd[0]); // Close unused read end
        printf("I'm the child !\n");
        child_process(pipe_fd[1]);
        close(pipe_fd[1]);
    } else {
        // Parent process
        close(pipe_fd[1]); // Close unused write end
        printf("I'm the parent !\n");
        parent_process(pipe_fd[0]);
        close(pipe_fd[0]);
    }

    return 0;
}
