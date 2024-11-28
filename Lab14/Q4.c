#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define ARRAY_SIZE 100
#define NUM_THREADS 4

int array[ARRAY_SIZE];
int global_min;
sem_t min_semaphore;

void* find_min(void* arg) {
    int thread_id = *(int*)arg;
    int segment_size = ARRAY_SIZE / NUM_THREADS;
    int start_index = thread_id * segment_size;
    int end_index = (thread_id == NUM_THREADS - 1) ? ARRAY_SIZE : start_index + segment_size;

    // Find local minimum in the segment
    int local_min = array[start_index];
    for (int i = start_index + 1; i < end_index; i++) {
        if (array[i] < local_min) {
            local_min = array[i];
        }
    }

    // Update global minimum with semaphore protection
    sem_wait(&min_semaphore);
    if (local_min < global_min) {
        global_min = local_min;
    }
    sem_post(&min_semaphore);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];

    // Initialize array with random values
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 1000;  // Random values between 0 and 999
    }

    // Initialize global minimum
    global_min = array[0];

    // Initialize semaphore
    sem_init(&min_semaphore, 0, 1);

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, find_min, &thread_args[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy semaphore
    sem_destroy(&min_semaphore);

    // Display the minimum value
    printf("Minimum value in the array: %d\n", global_min);

    return 0;
}