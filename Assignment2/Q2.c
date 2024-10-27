#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1
#define MAX_BUF 1024

int create_process_chain(int process_num, int total_processes) {
    if (process_num >= total_processes) {
        return 0;
    }

    int pipe_to_next[2];
    int pipe_from_last[2];
    
    // Create pipes
    if (pipe(pipe_to_next) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }
    if (process_num == total_processes - 1) {
        if (pipe(pipe_from_last) == -1) {
            perror("Pipe creation failed");
            exit(1);
        }
    }

    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    
    if (pid == 0) { // Child process
        // Close unused pipe ends
        if (process_num > 0) {
            close(STDIN_FILENO);
            dup2(3, STDIN_FILENO);  // Previous pipe becomes stdin
            close(3);
        }
        
        close(STDOUT_FILENO);
        dup2(pipe_to_next[WRITE_END], STDOUT_FILENO);
        close(pipe_to_next[READ_END]);
        close(pipe_to_next[WRITE_END]);

        char buffer[MAX_BUF];
        while (1) {
            if (fgets(buffer, MAX_BUF, stdin) == NULL) {
                exit(0);
            }
            
            buffer[strcspn(buffer, "\n")] = 0;  // Remove newline
            
            if (strcmp(buffer, "Quit") == 0) {
                printf("Quit\n");
                fflush(stdout);
                exit(0);
            }
            
            printf("%s:%d\n", buffer, getpid());
            fflush(stdout);
        }
        
    } else { // Parent process
        if (process_num > 0) {
            close(3);  // Close the previous pipe
        }
        
        if (process_num == 0) { // P1 process
            close(pipe_to_next[WRITE_END]);
            
            char input[MAX_BUF];
            while (1) {
                printf("Enter message (or 'Quit' to exit): ");
                fflush(stdout);
                
                if (fgets(input, MAX_BUF, stdin) == NULL) {
                    break;
                }
                
                input[strcspn(input, "\n")] = 0;  // Remove newline
                
                if (strcmp(input, "Quit") == 0) {
                    printf("Quit\n");
                    break;
                }
                
                printf("%s:%d\n", input, getpid());
                fflush(stdout);
                
                // Send to next process
                char msg[MAX_BUF];
                sprintf(msg, "%s:%d\n", input, getpid());
                write(4, msg, strlen(msg));  // Write to next process
                
                // Read final result
                char result[MAX_BUF];
                FILE* fp = fdopen(pipe_to_next[READ_END], "r");
                if (fgets(result, MAX_BUF, fp) != NULL) {
                    printf("%s", result);
                    fflush(stdout);
                }
            }
        } else {
            dup2(pipe_to_next[READ_END], 3);  // Save pipe for next child
            close(pipe_to_next[READ_END]);
            close(pipe_to_next[WRITE_END]);
        }
        
        // Create next process in chain
        return create_process_chain(process_num + 1, total_processes);
    }
    return 0;
}

int main() {
    create_process_chain(0, 6);  // Create chain of 6 processes (P1 + C1-C5)
    return 0;
}