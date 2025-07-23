// deadlock_creation.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int id;
    pthread_mutex_t lock;
} Account;

Account A = { .id = 1, .lock = PTHREAD_MUTEX_INITIALIZER };
Account B = { .id = 2, .lock = PTHREAD_MUTEX_INITIALIZER };

void* transfer_A_to_B(void* arg) {
    pthread_mutex_lock(&A.lock);
    printf("Thread 1 locked Account A\n");
    sleep(1); // Delay to increase chance of deadlock
    pthread_mutex_lock(&B.lock);
    printf("Thread 1 locked Account B\n");
    pthread_mutex_unlock(&B.lock);
    pthread_mutex_unlock(&A.lock);
    return NULL;
}

void* transfer_B_to_A(void* arg) {
    pthread_mutex_lock(&B.lock);
    printf("Thread 2 locked Account B\n");
    sleep(1);
    pthread_mutex_lock(&A.lock);
    printf("Thread 2 locked Account A\n");
    pthread_mutex_unlock(&A.lock);
    pthread_mutex_unlock(&B.lock);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, transfer_A_to_B, NULL);
    pthread_create(&t2, NULL, transfer_B_to_A, NULL);

    pthread_join(t1, NULL); // May never complete if deadlock happens
    pthread_join(t2, NULL);
    return 0;
}

