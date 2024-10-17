#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    if (signum == SIGTERM) {
        printf("Received SIGTERM. Cleaning up and exiting gracefully.\n");
        exit(0);
    }
}

int main() {
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Failed to install SIGTERM handler");
        return 1;
    }

    if (sigaction(SIGKILL, &sa, NULL) == -1) {
        printf("Failed to install SIGKILL handler. This is expected as SIGKILL cannot be caught.\n");
    }

    printf("Process ID: %d\n", getpid());
    printf("This program will handle SIGTERM gracefully but cannot handle SIGKILL.\n");
    printf("Try the following commands in another terminal:\n");
    printf("  kill -SIGTERM %d  (The program will exit gracefully)\n", getpid());
    printf("  kill -SIGKILL %d  (The program will be terminated immediately)\n", getpid());

    while (1) {
        printf("Running...\n");
        sleep(2);
    }

    return 0;
}