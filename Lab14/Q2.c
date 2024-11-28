#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ITEMS 5

// Shared buffer
int buffer = -1;

// Semaphores
sem_t empty;  // Indicates buffer is empty
sem_t full;   // Indicates buffer is full

void* producer(void* arg) {
    for(int i = 0; i < NUM_ITEMS; i++) {
        // Produce a random number
        int item = rand() % 100;

        // Wait if buffer is not empty
        sem_wait(&empty);

        // Put item into buffer
        buffer = item;
        printf("Producer produced: %d\n", item);

        // Signal that buffer is full
        sem_post(&full);

        // Sleep to simulate production time
        usleep(500000);
    }
    return NULL;
}

void* consumer(void* arg) {
    for(int i = 0; i < NUM_ITEMS; i++) {
        // Wait if buffer is empty
        sem_wait(&full);

        // Consume item from buffer
        int item = buffer;
        printf("Consumer consumed: %d\n", item);
        buffer = -1;  // Reset buffer

        // Signal that buffer is empty
        sem_post(&empty);

        // Sleep to simulate consumption time
        usleep(800000);
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    srand(time(NULL));

    sem_init(&empty, 0, 1);  // Buffer starts empty
    sem_init(&full, 0, 0);   // Buffer starts not full

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}