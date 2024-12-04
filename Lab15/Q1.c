#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;

// Semaphores for synchronization
sem_t empty; // Tracks empty slots in the buffer
sem_t full;  // Tracks filled slots in the buffer
pthread_mutex_t mutex; // Protects access to the buffer

void* producer(void* arg) {
    for (int i = 0; i < 10; i++) {
        sem_wait(&empty);               // Wait if buffer is full
        pthread_mutex_lock(&mutex);     // Lock buffer access

        buffer[count++] = i;            // Produce an item
        printf("Produced item: %d\n", i);

        pthread_mutex_unlock(&mutex);   // Unlock buffer access
        sem_post(&full);                // Signal that buffer has an item

        sleep(1); // Simulate production delay
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < 10; i++) {
        sem_wait(&full);                // Wait if buffer is empty
        pthread_mutex_lock(&mutex);     // Lock buffer access

        int item = buffer[--count];     // Consume an item
        printf("Consumed item: %d\n", item);

        pthread_mutex_unlock(&mutex);   // Unlock buffer access
        sem_post(&empty);               // Signal that buffer has an empty slot

        sleep(2); // Simulate consumption delay
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE); // BUFFER_SIZE empty slots initially
    sem_init(&full, 0, 0);           // 0 full slots initially
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for threads to finish
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Destroy semaphores and mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
