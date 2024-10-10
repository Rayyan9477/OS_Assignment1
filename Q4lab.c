#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

volatile sig_atomic_t keep_running = 1;
volatile sig_atomic_t count = 0;

void signal_handler(int signum) {
    switch (signum) {
        case SIGINT:
            printf("\nReceived SIGINT. Current count: %d\n", count);
            break;
        case SIGTERM:
            printf("\nReceived SIGTERM. Final count: %d\n", count);
            keep_running = 0;
            break;
    }
}

int main() {
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Failed to install SIGINT handler");
        return 1;
    }

    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Failed to install SIGTERM handler");
        return 1;
    }

    printf("Process ID: %d\n", getpid());
    printf("Counting numbers. Send SIGINT to see current count or SIGTERM to exit.\n");

    while (keep_running) {
        count++;
        usleep(1000);  // Sleep for 1 millisecond to reduce CPU usage
    }

    printf("Program terminated. Final count: %d\n", count);
    return 0;
}