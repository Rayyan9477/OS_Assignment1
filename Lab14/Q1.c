#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Global semaphores
sem_t sem1, sem2;

void* thread1_function(void* arg) {
    for(int i = 0; i < 5; i++) {
        sem_wait(&sem1);
        printf("Thread 1 is printing message %d\n", i + 1);
        sem_post(&sem2);
        usleep(100000); // Small delay to make output readable
    }
    return NULL;
}

void* thread2_function(void* arg) {
    for(int i = 0; i < 5; i++) {
        sem_wait(&sem2);
        printf("Thread 2 is printing message %d\n", i + 1);
        sem_post(&sem1);
        usleep(100000); // Small delay to make output readable
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    
    // Initialize semaphores
    sem_init(&sem1, 0, 1);  // sem1 starts at 1 to let thread1 go first
    sem_init(&sem2, 0, 0);  // sem2 starts at 0
    
    // Create threads
    pthread_create(&thread1, NULL, thread1_function, NULL);
    pthread_create(&thread2, NULL, thread2_function, NULL);
    
    // Wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    // Cleanup semaphores
    sem_destroy(&sem1);
    sem_destroy(&sem2);
    
    return 0;
}