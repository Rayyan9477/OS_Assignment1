#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 4
#define NUM_TRANSACTIONS 5

// Shared resources
double account_balance = 1000.0;
pthread_mutex_t balance_mutex;

void* perform_transactions(void* thread_id) {
    long tid = (long)thread_id;
    
    for (int i = 0; i < NUM_TRANSACTIONS; i++) {
        // Simulate some work before transaction
        usleep(50000);  // 50ms delay
        
        // Random amount between -100 and 100
        double amount = ((double)rand() / RAND_MAX) * 200 - 100;
        
        // Lock the mutex before accessing shared resource
        pthread_mutex_lock(&balance_mutex);
        
        double old_balance = account_balance;
        account_balance += amount;
        
        printf("Thread %ld: %s $%.2f, Balance: $%.2f -> $%.2f\n", 
               tid,
               (amount >= 0) ? "Deposit" : "Withdraw",
               (amount >= 0) ? amount : -amount,
               old_balance,
               account_balance);
        
        // Unlock the mutex
        pthread_mutex_unlock(&balance_mutex);
        
        // Simulate some work after transaction
        usleep(30000);  // 30ms delay
    }
    
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    
    // Seed random number generator
    srand(time(NULL));
    
    // Initialize mutex
    if (pthread_mutex_init(&balance_mutex, NULL) != 0) {
        printf("Mutex initialization failed\n");
        return 1;
    }
    
    printf("Initial balance: $%.2f\n", account_balance);
    
    // Create threads
    for(t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, perform_transactions, (void*)t);
        if (rc) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            return 1;
        }
    }
    
    // Wait for threads to finish
    for(t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }
    
    // Destroy mutex
    pthread_mutex_destroy(&balance_mutex);
    
    printf("Final balance: $%.2f\n", account_balance);
    
    return 0;
}