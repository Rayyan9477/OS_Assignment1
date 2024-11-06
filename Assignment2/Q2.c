#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_MSG 1024
#define PROC_COUNT 6

void create_process(int read_fd, int write_fd, int level);

int main() {
    int pipes[PROC_COUNT][2];
    pid_t pids[PROC_COUNT-1];  // Array to store child PIDs

    // Create pipes
    for (int i = 0; i < PROC_COUNT; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    // Create child processes
    for (int i = 0; i < PROC_COUNT-1; i++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            exit(1);
        }
        if (pids[i] == 0) {  // Child process
            // Close all unused pipes
            for (int j = 0; j < PROC_COUNT; j++) {
                if (j == i) {  // Input pipe
                    close(pipes[j][1]);
                } else if (j == i + 1) {  // Output pipe
                    close(pipes[j][0]);
                } else {  // Unused pipes
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
            }
            
            create_process(pipes[i][0], pipes[i+1][1], i+1);
            exit(0);
        }
    }

    // Parent process (P1)
    // Close unused pipe ends
    for (int i = 0; i < PROC_COUNT; i++) {
        if (i == 0) {
            close(pipes[i][0]);
        } else if (i == PROC_COUNT-1) {
            close(pipes[i][1]);
        } else {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
    }

    char input[MAX_MSG];
    while (1) {
        printf("Enter message (Quit for exit): ");
        fflush(stdout);

        if (fgets(input, MAX_MSG, stdin) == NULL) break;
        
        // Remove newline
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "Quit") == 0) {
            write(pipes[0][1], "Quit\n", 5);
            break;
        }

        // Add own PID and send
        char buffer[MAX_MSG];
        snprintf(buffer, MAX_MSG, "%s:%d\n", input, getpid());
        printf("%s", buffer);
        write(pipes[0][1], buffer, strlen(buffer));

        // Read final result
        char final_buffer[MAX_MSG];
        ssize_t n = read(pipes[PROC_COUNT-1][0], final_buffer, MAX_MSG-1);
        if (n > 0) {
            final_buffer[n] = '\0';
            if (strncmp(final_buffer, "Quit", 4) != 0) {
                printf("%s", final_buffer);
            }
        }
    }

    // Wait for all children to finish
    for (int i = 0; i < PROC_COUNT-1; i++) {
        waitpid(pids[i], NULL, 0);
    }

    // Close remaining pipes
    close(pipes[0][1]);
    close(pipes[PROC_COUNT-1][0]);

    return 0;
}

void create_process(int read_fd, int write_fd, int level) {
    char buffer[MAX_MSG];
    while (1) {
        ssize_t n = read(read_fd, buffer, MAX_MSG-1);
        if (n <= 0) break;
        
        buffer[n] = '\0';
        if (strncmp(buffer, "Quit", 4) == 0) {
            write(write_fd, "Quit\n", 5);
            exit(0);
        }

        // Remove newline if present
        if (buffer[n-1] == '\n') buffer[n-1] = '\0';

        // Append PID
        char new_buffer[MAX_MSG];
        snprintf(new_buffer, MAX_MSG, "%s:%d\n", buffer, getpid());
        
        // Print and forward message
        printf("%s", new_buffer);
        fflush(stdout);
        write(write_fd, new_buffer, strlen(new_buffer));
    }
    exit(0);
}