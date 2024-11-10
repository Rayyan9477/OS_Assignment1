#include "common.h"
#include <signal.h>

volatile sig_atomic_t signal_received = 0;
volatile sig_atomic_t should_exit = 0;
int fd_write = -1, fd_read = -1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

typedef struct {
    pthread_t thread;
    int active;
} ThreadControl;

ThreadControl threads[3]; // For our three threads

void cleanup(void) {
    // Close pipes
    if (fd_write != -1) close(fd_write);
    if (fd_read != -1) close(fd_read);
    
    // Remove pipes
    unlink(PIPE_A_TO_B);
    unlink(PIPE_B_TO_A);
    
    // Remove PID file
    unlink(PID_FILE);
    
    // Destroy mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

void signal_handler(int signo) {
    if (signo == SIGUSR1) {
        pthread_mutex_lock(&mutex);
        signal_received = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    } else if (signo == SIGINT || signo == SIGTERM) {
        should_exit = 1;
        // Signal all threads to exit
        for (int i = 0; i < 3; i++) {
            if (threads[i].active) {
                pthread_cancel(threads[i].thread);
            }
        }
    }
}

void* message_generator(void* arg) {
    char message[MAX_MSG_SIZE];
    int msg_count = 0;

    while (!should_exit && msg_count < NUM_MESSAGES) {
        snprintf(message, MAX_MSG_SIZE, "Message %d from Process A", msg_count + 1);
        
        ssize_t bytes_written = write(fd_write, message, strlen(message) + 1);
        if (bytes_written > 0) {
            printf("Process A Generator: Sent message: %s\n", message);
            msg_count++;
        } else if (bytes_written == -1 && errno != EINTR) {
            perror("Write error");
            break;
        }
        
        sleep(1); // Delay between messages
    }

    should_exit = 1; // Signal other threads to exit
    pthread_exit(NULL);
}

void* message_handler(void* arg) {
    char buffer[MAX_MSG_SIZE];

    while (!should_exit) {
        ssize_t bytes_read = read(fd_read, buffer, MAX_MSG_SIZE);
        
        if (bytes_read > 0) {
            printf("Process A Handler: Received processed message: %s\n", buffer);
        } else if (bytes_read == -1 && errno != EINTR) {
            if (should_exit) break;
            perror("Read error");
            continue;
        }
    }

    pthread_exit(NULL);
}

void* signal_waiter(void* arg) {
    while (!should_exit) {
        pthread_mutex_lock(&mutex);
        while (!signal_received && !should_exit) {
            pthread_cond_wait(&cond, &mutex);
        }
        
        if (signal_received) {
            printf("Process A: Received SIGUSR1 signal from Process B\n");
            signal_received = 0;
        }
        
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}


// gcc -o process_a process_a.c -lpthread

int main() {
    // Register signal handlers
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    CHECK(sigaction(SIGUSR1, &sa, NULL) != -1);
    CHECK(sigaction(SIGINT, &sa, NULL) != -1);
    CHECK(sigaction(SIGTERM, &sa, NULL) != -1);

    // Register cleanup function
    atexit(cleanup);

    // Create named pipes
    CHECK(mkfifo(PIPE_A_TO_B, 0666) != -1 || errno == EEXIST);
    CHECK(mkfifo(PIPE_B_TO_A, 0666) != -1 || errno == EEXIST);

    // Save PID for Process B
    FILE* pid_file = fopen(PID_FILE, "w");
    CHECK(pid_file != NULL);
    fprintf(pid_file, "%d", getpid());
    fclose(pid_file);

    // Open pipes
    printf("Process A: Opening pipes...\n");
    fd_write = open(PIPE_A_TO_B, O_WRONLY);
    CHECK(fd_write != -1);
    fd_read = open(PIPE_B_TO_A, O_RDONLY);
    CHECK(fd_read != -1);
    printf("Process A: Pipes opened successfully\n");

    // Create threads
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Start threads
    CHECK(pthread_create(&threads[0].thread, &attr, message_generator, NULL) == 0);
    threads[0].active = 1;
    
    CHECK(pthread_create(&threads[1].thread, &attr, message_handler, NULL) == 0);
    threads[1].active = 1;
    
    CHECK(pthread_create(&threads[2].thread, &attr, signal_waiter, NULL) == 0);
    threads[2].active = 1;

    pthread_attr_destroy(&attr);

    // Wait for message generator to complete
    pthread_join(threads[0].thread, NULL);
    threads[0].active = 0;

    // Set exit flag and wait for other threads
    should_exit = 1;
    pthread_cond_broadcast(&cond);

    for (int i = 1; i < 3; i++) {
        if (threads[i].active) {
            pthread_join(threads[i].thread, NULL);
            threads[i].active = 0;
        }
    }

    printf("Process A: All threads completed, exiting...\n");
    return 0;
}