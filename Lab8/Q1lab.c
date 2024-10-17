#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum);

int main() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("Failed to install SIGINT handler");
        return 1;
    }
    if (signal(SIGTERM, signal_handler) == SIG_ERR) {
        perror("Failed to install SIGTERM handler");
        return 1;
    }
    if (signal(SIGALRM, signal_handler) == SIG_ERR) {
        perror("Failed to install SIGALRM handler");
        return 1;
    }

    printf("Process ID: %d\n", getpid());
    printf("Waiting for signals. Use 'kill -SIGINT %d', 'kill -SIGTERM %d', or 'kill -SIGALRM %d' to send signals.\n", getpid(), getpid(), getpid());

    while (1) {
        pause();
    }

    return 0;
}


void signal_handler(int signum) {
    switch (signum) {
        case SIGINT:
            printf("\nReceived SIGINT (Ctrl+C). Ignoring...\n");
            break;
        case SIGTERM:
            printf("Received SIGTERM. Terminating gracefully...\n");
            exit(0);
        case SIGALRM:
            printf("Received SIGALRM. Alarm triggered!\n");
            break;
        default:
            printf("Received unexpected signal: %d\n", signum);
    }
}