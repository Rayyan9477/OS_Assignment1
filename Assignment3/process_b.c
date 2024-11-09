// process_b.c
#include "common.h"

volatile sig_atomic_t should_exit = 0;
int fd_read = -1, fd_write = -1;

void cleanup(void) {
    if (fd_read != -1) close(fd_read);
    if (fd_write != -1) close(fd_write);
}

void signal_handler(int signo) {
    if (signo == SIGINT || signo == SIGTERM) {
        should_exit = 1;
    }
}

void transform_message(char* message) {
    for (int i = 0; message[i]; i++) {
        message[i] = toupper(message[i]);
    }
}

int main() {
    // Register signal handlers
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    CHECK(sigaction(SIGINT, &sa, NULL) != -1);
    CHECK(sigaction(SIGTERM, &sa, NULL) != -1);

    // Register cleanup function
    atexit(cleanup);

    // Get Process A's PID
    pid_t process_a_pid;
    FILE* pid_file;
    int retry_count = 0;
    const int max_retries = 5;

    while (retry_count < max_retries) {
        pid_file = fopen(PID_FILE, "r");
        if (pid_file) break;
        sleep(1);
        retry_count++;
    }

    CHECK(pid_file != NULL);
    CHECK(fscanf(pid_file, "%d", &process_a_pid) == 1);
    fclose(pid_file);

    printf("Process B: Got Process A's PID: %d\n", process_a_pid);

    // Open pipes
    printf("Process B: Opening pipes...\n");
    fd_read = open(PIPE_A_TO_B, O_RDONLY);
    CHECK(fd_read != -1);
    fd_write = open(PIPE_B_TO_A, O_WRONLY);
    CHECK(fd_write != -1);
    printf("Process B: Pipes opened successfully\n");

    char buffer[MAX_MSG_SIZE];
    int msg_count = 0;

    while (!should_exit && msg_count < NUM_MESSAGES) {
        ssize_t bytes_read = read(fd_read, buffer, MAX_MSG_SIZE);
        
        if (bytes_read > 0) {
            printf("Process B: Received message: %s\n", buffer);
            msg_count++;

            // Process the message
            transform_message(buffer);
            printf("Process B: Transformed message: %s\n", buffer);

            // Send processed message back
            ssize_t bytes_written = write(fd_write, buffer, strlen(buffer) + 1);
            if (bytes_written > 0) {
                // Signal Process A
                if (kill(process_a_pid, SIGUSR1) == 0) {
                    printf("Process B: Sent SIGUSR1 signal to Process A\n");
                } else {
                    perror("Failed to send signal");
                    break;
                }
            } else if (bytes_written == -1 && errno != EINTR) {
                perror("Write error");
                break;
            }
        } else if (bytes_read == -1 && errno != EINTR) {
            if (should_exit) break;
            perror("Read error");
            continue;
        }
    }

    printf("Process B: Processing complete, exiting...\n");
    return 0;
}