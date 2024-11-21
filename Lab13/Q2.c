#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_TRANSFERS 5

// Account structure
typedef struct {
    int id;
    double balance;
    pthread_mutex_t lock;
} Account;

// Global accounts
Account account1 = {1, 1000.0};
Account account2 = {2, 1000.0};

// Safe transfer function that prevents deadlock
void* transfer_money(void* arg) {
    int direction = *(int*)arg;
    Account *from, *to;
    
    // Determine transfer direction based on thread
    if (direction == 1) {
        from = &account1;
        to = &account2;
    } else {
        from = &account2;
        to = &account1;
    }
    
    for (int i = 0; i < NUM_TRANSFERS; i++) {
        // Always lock accounts in order of ascending ID to prevent deadlock
        Account *first = (from->id < to->id) ? from : to;
        Account *second = (from->id < to->id) ? to : from;
        
        pthread_mutex_lock(&first->lock);
        pthread_mutex_lock(&second->lock);
        
        double amount = (rand() % 100) + 1;
        
        if (from->balance >= amount) {
            from->balance -= amount;
            to->balance += amount;
            printf("Transfer $%.2f from Account %d to Account %d\n", 
                   amount, from->id, to->id);
            printf("Account %d balance: $%.2f\n", from->id, from->balance);
            printf("Account %d balance: $%.2f\n", to->id, to->balance);
        }
        
        pthread_mutex_unlock(&second->lock);
        pthread_mutex_unlock(&first->lock);
        
        // Sleep to simulate processing time
        usleep(100000);
    }
    
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int dir1 = 1, dir2 = 2;
    
    // Initialize mutexes
    pthread_mutex_init(&account1.lock, NULL);
    pthread_mutex_init(&account2.lock, NULL);
    
    // Seed random number generator
    srand(time(NULL));
    
    printf("Initial balances:\n");
    printf("Account 1: $%.2f\n", account1.balance);
    printf("Account 2: $%.2f\n", account2.balance);
    
    // Create threads
    pthread_create(&thread1, NULL, transfer_money, &dir1);
    pthread_create(&thread2, NULL, transfer_money, &dir2);
    
    // Wait for threads to complete
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("\nFinal balances:\n");
    printf("Account 1: $%.2f\n", account1.balance);
    printf("Account 2: $%.2f\n", account2.balance);
    
    // Cleanup
    pthread_mutex_destroy(&account1.lock);
    pthread_mutex_destroy(&account2.lock);
    
    return 0;
}