#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

#define TIMEOUT_SECONDS 2

void* thread1_function(void* arg) {
    struct timespec timeout;
    
    printf("Thread 1 trying to lock mutex1\n");
    pthread_mutex_lock(&mutex1);
    printf("Thread 1 locked mutex1\n");
    
    // Sleep to increase chances of deadlock
    sleep(1);
    
    printf("Thread 1 trying to lock mutex2\n");
    
    // Set timeout
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += TIMEOUT_SECONDS;
    
    if (pthread_mutex_timedlock(&mutex2, &timeout) != 0) {
        printf("Thread 1 couldn't lock mutex2 - timeout occurred\n");
        pthread_mutex_unlock(&mutex1);
        return NULL;
    }
    
    printf("Thread 1 locked mutex2\n");
    
    // Critical section
    printf("Thread 1 in critical section\n");
    
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    
    return NULL;
}

void* thread2_function(void* arg) {
    struct timespec timeout;
    
    printf("Thread 2 trying to lock mutex2\n");
    pthread_mutex_lock(&mutex2);
    printf("Thread 2 locked mutex2\n");
    
    // Sleep to increase chances of deadlock
    sleep(1);
    
    printf("Thread 2 trying to lock mutex1\n");
    
    // Set timeout
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += TIMEOUT_SECONDS;
    
    if (pthread_mutex_timedlock(&mutex1, &timeout) != 0) {
        printf("Thread 2 couldn't lock mutex1 - timeout occurred\n");
        pthread_mutex_unlock(&mutex2);
        return NULL;
    }
    
    printf("Thread 2 locked mutex1\n");
    
    // Critical section
    printf("Thread 2 in critical section\n");
    
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    
    // Create threads
    pthread_create(&thread1, NULL, thread1_function, NULL);
    pthread_create(&thread2, NULL, thread2_function, NULL);
    
    // Wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("Program completed\n");
    
    // Cleanup
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    
    return 0;
}