#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void sigchld_handler(int signum) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0) {
        printf("Parent: Child process %d has completed.\n", pid);
    }
}

int main() {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        printf("Child: I'm the child process (PID: %d)\n", getpid());
        sleep(2);
        printf("Child: Exiting now\n");
        exit(0);
    } else {
        printf("Parent: Created child process with PID: %d\n", pid);
        printf("Parent: Waiting for child to complete...\n");
        
        while(1) {
            sleep(1);
        }
    }

    return 0;
}