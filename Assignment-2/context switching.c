#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile int switch_flag = 0; // Flag to indicate when to switch processes

// Signal handler for the alarm signal
void alarm_handler(int signum) {
    switch_flag = 1;
}

// Function to simulate process A
void process_A() {
    while (1) {
        printf("Process A is running\n");
        sleep(1);
        if (switch_flag) {
            switch_flag = 0;
            kill(getpid(), SIGSTOP); // Stop this process
        }
    }
}

// Function to simulate process B
void process_B() {
    while (1) {
        printf("Process B is running\n");
        sleep(1);
        if (switch_flag) {
            switch_flag = 0;
            kill(getpid(), SIGSTOP); // Stop this process
        }
    }
}

int main() {
    // Set up the alarm signal handler
    signal(SIGALRM, alarm_handler);

    // Create two child processes
    pid_t pid_A, pid_B;
    pid_A = fork();

    if (pid_A < 0) {
        perror("Failed to create Process A");
        exit(1);
    } else if (pid_A == 0) {
        // This is Process A
        process_A();
    } else {
        pid_B = fork();

        if (pid_B < 0) {
            perror("Failed to create Process B");
            exit(1);
        } else if (pid_B == 0) {
            // This is Process B
            process_B();
        } else {
            // This is the parent process
            while (1) {
                // Set an alarm to switch processes every 2 seconds
                alarm(2);
                pause(); // Wait for the alarm signal to be received
            }
        }
    }

    return 0;
}
