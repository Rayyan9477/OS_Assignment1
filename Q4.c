#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void create_process(const char* name) {
    pid_t pid = fork();
    
    if (pid < 0) {
        // Fork failed
        perror("fork failed");
    } else if (pid == 0) {
        // Child process
        printf("Process %s (PID: %d, Parent PID: %d)\n", name, getpid(), getppid());
    } else {
        // Parent process waits for the child to finish
        wait(NULL);
    }
}

int main() {
    pid_t pid;

    // Root node 'n'
    printf("Root Process n (PID: %d)\n", getpid());

    // Create process m
    pid = fork();
    if (pid == 0) {
        create_process("m");

        // Create child processes under 'm'
        pid_t pid2 = fork();
        if (pid2 == 0) {
            create_process("C0");
        } else {
            wait(NULL);
            pid2 = fork();
            if (pid2 == 0) {
                create_process("C1");
            } else {
                wait(NULL);
                pid2 = fork();
                if (pid2 == 0) {
                    create_process("C2");
                } else {
                    wait(NULL);
                }
            }
        }
    } else {
        wait(NULL);
        // Create process C1
        pid = fork();
        if (pid == 0) {
            create_process("C1");

            // Create child processes under 'C1'
            pid_t pid3 = fork();
            if (pid3 == 0) {
                create_process("C3");
            } else {
                wait(NULL);
                pid3 = fork();
                if (pid3 == 0) {
                    create_process("C4");
                } else {
                    wait(NULL);
                }
            }
        } else {
            wait(NULL);
        }

        // Create process C2
        pid = fork();
        if (pid == 0) {
            create_process("C2");

            // Create child processes under 'C2'
            pid_t pid4 = fork();
            if (pid4 == 0) {
                create_process("C5");
            } else {
                wait(NULL);
                pid4 = fork();
                if (pid4 == 0) {
                    create_process("C6");
                } else {
                    wait(NULL);
                }
            }
        } else {
            wait(NULL);
        }
    }

    return 0;
}
