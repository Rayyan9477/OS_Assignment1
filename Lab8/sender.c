#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <PID> <signal_name_or_number>\n", argv[0]);
        return 1;
    }

    pid_t pid = atoi(argv[1]);
    int sig;

    if (strncmp(argv[2], "SIG", 3) == 0) {
        if (strcmp(argv[2], "SIGUSR1") == 0) sig = SIGUSR1;
        else if (strcmp(argv[2], "SIGUSR2") == 0) sig = SIGUSR2;
        else {
            fprintf(stderr, "Unsupported signal name: %s\n", argv[2]);
            return 1;
        }
    } else {
        sig = atoi(argv[2]);
    }

    if (kill(pid, sig) == -1) {
        perror("kill");
        return 1;
    }

    printf("Signal %d sent to process %d\n", sig, pid);
    return 0;
}