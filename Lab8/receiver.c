#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    if (signum == SIGUSR1) {
        printf("Received SIGUSR1\n");
    } else if (signum == SIGUSR2) {
        printf("Received SIGUSR2\n");
    }
}

int main() {
    if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
        perror("Failed to install SIGUSR1 handler");
        return 1;
    }

    if (signal(SIGUSR2, signal_handler) == SIG_ERR) {
        perror("Failed to install SIGUSR2 handler");
        return 1;
    }

    printf("Process ID: %d\n", getpid());
    printf("Waiting for SIGUSR1 or SIGUSR2...\n");

    while (1) {
        pause();
    }

    return 0;
}