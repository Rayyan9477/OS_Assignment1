#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define PARKING_SPOTS 5
#define NUM_CARS 8

// Semaphore to track parking spots
sem_t parking_spots;

// Mutex for synchronization
pthread_mutex_t mutex;

// Function executed by each car thread
void* car(void* arg) {
    int car_id = *(int*)arg;

    printf("Car %d is waiting for a parking spot.\n", car_id);

    // Wait for an available parking spot
    sem_wait(&parking_spots);

    // Lock the mutex to park the car
    pthread_mutex_lock(&mutex);
    printf("Car %d has parked.\n", car_id);
    pthread_mutex_unlock(&mutex);

    // Simulate parking
    sleep(3);

    // Lock mutex to simulate car leaving the spot
    pthread_mutex_lock(&mutex);
    printf("Car %d has left the parking spot.\n", car_id);
    pthread_mutex_unlock(&mutex);

    // Signal that a parking spot is free
    sem_post(&parking_spots);

    free(arg);
    return NULL;
}

int main() {
    pthread_t cars[NUM_CARS];

    // Initialize the semaphore and mutex
    sem_init(&parking_spots, 0, PARKING_SPOTS);
    pthread_mutex_init(&mutex, NULL);

    // Create car threads
    for (int i = 0; i < NUM_CARS; i++) {
        int* car_id = malloc(sizeof(int)); // Dynamically allocate memory for car ID
        *car_id = i + 1;
        pthread_create(&cars[i], NULL, car, car_id);
    }

    // Wait for all car threads to finish
    for (int i = 0; i < NUM_CARS; i++) {
        pthread_join(cars[i], NULL);
    }

    // Destroy the semaphore and mutex
    sem_destroy(&parking_spots);
    pthread_mutex_destroy(&mutex);

    return 0;
}
