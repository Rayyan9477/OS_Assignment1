#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> 

void *calculate_sum(void *arg) {
    int *array = (int *)arg;
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += array[i];
    }
    int *result = malloc(sizeof(int)); 
    *result = sum;
    pthread_exit(result); 
}

int main() {
    pthread_t thread;
    int array[5] = {1, 2, 3, 4, 5};
    int *result;

    pthread_create(&thread, NULL, calculate_sum, array);

    pthread_join(thread, (void **)&result); 

    printf("Sum of array elements: %d\n", *result);
    free(result); 

    return 0;
}