#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>  // Added for strcpy

void create_process(const char* name, int level);

int main() {
    printf("Root Process n (PID: %d)\n", getpid());
    create_process("m", 0);
    return 0;
}

void create_process(const char* name, int level) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Process %s (PID: %d, Parent PID: %d)\n", name, getpid(), getppid());
        
        if (level < 4) {  // We don't create children for level 4 (leaf nodes)
            if (level == 0) {
                create_process("m", level + 1);
                create_process("C2", level + 1);
            } else if (level == 1) {
                if (*name == 'm') {
                    create_process("m", level + 1);
                    create_process("C4", level + 1);
                } else if (*name == 'C') {
                    create_process("C1", level + 1);
                    create_process("C3", level + 1);
                }
            } else if (level == 2) {
                if (*name == 'm') {
                    create_process("m", level + 1);
                    create_process("C4", level + 1);
                } else if (*name == 'C') {
                    char child1[3], child2[3];
                    snprintf(child1, sizeof(child1), "C%d", (*name - '0') * 2 - 1);
                    snprintf(child2, sizeof(child2), "C%d", (*name - '0') * 2);
                    create_process(child1, level + 1);
                    create_process(child2, level + 1);
                }
            } else if (level == 3) {
                char child1[3], child2[3];
                if (*name == 'm') {
                    strcpy(child1, "C0");
                    strcpy(child2, "C1");
                } else {
                    snprintf(child1, sizeof(child1), "C%d", (*name - '0') * 2 - 1);
                    snprintf(child2, sizeof(child2), "C%d", (*name - '0') * 2);
                }
                create_process(child1, level + 1);
                create_process(child2, level + 1);
            }
        }
        exit(0);
    } else {
        // Parent process
        wait(NULL);  // Wait for the child to terminate
    }
}