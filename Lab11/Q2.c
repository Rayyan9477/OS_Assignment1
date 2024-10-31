#include <stdio.h>
#include <pthread.h>

void *calculate_sum(void *arg) {
    int *array = (int *)arg;
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += array[i];
    }
    printf("Sum of array elements: %d\n", sum);
    return NULL;
}

int main() {
    pthread_t thread;
    int array[5] = {1, 2, 3, 4, 5};

    pthread_create(&thread, NULL, calculate_sum, array);

    pthread_join(thread, NULL);

    return 0;
}