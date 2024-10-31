#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void *print(void *arg) {
    int thread_num = *(int *)arg;
    printf("Hello from thread %d\n", thread_num);
    return NULL;
}

int main() {
    pthread_t thread[5];
    int thread_arg[5]; 


    for (int i = 0; i < 5; i++) {
        thread_arg[i]= i+1;
        pthread_create(&thread[i], NULL, print, &thread_arg[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(thread[i], NULL);
    }

    printf("Hello from main thread\n");

    return 0;
}