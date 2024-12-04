#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_CONCURRENT_REQUESTS 3
#define NUM_CLIENTS 5

// Shared variables
int active_clients = 0;  // Tracks the number of active client requests
pthread_mutex_t mutex;   // Mutex for synchronization
pthread_cond_t cond;     // Condition variable

// Function executed by each client thread
void* client_request(void* arg) {
    int client_id = *(int*)arg;

    // Acquire the mutex
    pthread_mutex_lock(&mutex);

    // Wait until the server can handle the request
    while (active_clients >= MAX_CONCURRENT_REQUESTS) {
        printf("Client %d is waiting for the server to become available.\n", client_id);
        pthread_cond_wait(&cond, &mutex);
    }

    // Increment active client count
    active_clients++;
    printf("Client %d is being served by the server.\n", client_id);

    // Release the mutex
    pthread_mutex_unlock(&mutex);

    // Simulate request processing
    sleep(2);

    // Request processing complete, release the slot
    pthread_mutex_lock(&mutex);
    active_clients--;
    printf("Client %d has finished its request. Server is now available.\n", client_id);

    // Signal other waiting clients
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    free(arg);
    return NULL;
}

int main() {
    pthread_t clients[NUM_CLIENTS];

    // Initialize mutex and condition variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // Create threads for clients
    for (int i = 0; i < NUM_CLIENTS; i++) {
        int* client_id = malloc(sizeof(int)); // Allocate memory for client ID
        *client_id = i + 1;
        pthread_create(&clients[i], NULL, client_request, client_id);
    }

    // Wait for all client threads to finish
    for (int i = 0; i < NUM_CLIENTS; i++) {
        pthread_join(clients[i], NULL);
    }

    // Destroy mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
