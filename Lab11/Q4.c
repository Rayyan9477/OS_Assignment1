#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> 

void *find_max(void *arg) {
    int *array = (int *)arg;
    int *max_value = malloc(sizeof(int)); 
    *max_value = array[0];
    for (int i = 1; i < 5; i++) {
        if (array[i] > *max_value) {
            *max_value = array[i];
        }
    }
    return max_value; 
}

// Main program
int main() {
    pthread_t thread;
    int array[5] = {1, 2, 3, 4, 5};
    int *max_value;

    pthread_create(&thread, NULL, find_max, array);

    pthread_join(thread, (void **)&max_value); 

    printf("Maximum value in the array: %d\n", *max_value);
    free(max_value); 
    return 0;
}