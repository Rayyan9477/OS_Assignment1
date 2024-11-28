#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define ARRAY_SIZE 100
#define NUM_THREADS 4

int array[ARRAY_SIZE];
int total_sum = 0;
sem_t sum_semaphore;

void* sum_segment(void* arg) {
    int thread_id = *(int*)arg;
    int segment_size = ARRAY_SIZE / NUM_THREADS;
    int start_index = thread_id * segment_size;
    int end_index = (thread_id == NUM_THREADS - 1) ? ARRAY_SIZE : start_index + segment_size;
    int partial_sum = 0;

    // Calculate partial sum
    for (int i = start_index; i < end_index; i++) {
        partial_sum += array[i];
    }

    // Update total sum safely
    sem_wait(&sum_semaphore);
    total_sum += partial_sum;
    sem_post(&sum_semaphore);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];

    // Initialize array
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;  // Example data
    }

    // Initialize semaphore
    sem_init(&sum_semaphore, 0, 1);

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, sum_segment, &thread_args[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy semaphore
    sem_destroy(&sum_semaphore);

    // Display total sum
    printf("Total Sum: %d\n", total_sum);

    return 0;
}