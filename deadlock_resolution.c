// deadlock_resolution source code implements a solution based on lock ordering
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

void lock_in_order(Account* a1, Account* a2) { // addressing is used to determine a lock order
    if (a1 < a2) {
        pthread_mutex_lock(&a1->lock);
        pthread_mutex_lock(&a2->lock);
    } else {
        pthread_mutex_lock(&a2->lock);
        pthread_mutex_lock(&a1->lock);
    }
}

void unlock_in_order(Account* a1, Account* a2) {
    pthread_mutex_unlock(&a1->lock);
    pthread_mutex_unlock(&a2->lock);
}

void* transfer_A_to_B(void* arg) {
    lock_in_order(&A, &B);
    printf("Thread 1 safely locked A and B\n");
    unlock_in_order(&A, &B);
    return NULL;
}

void* transfer_B_to_A(void* arg) {
    lock_in_order(&B, &A);
    printf("Thread 2 safely locked B and A\n");
    unlock_in_order(&B, &A);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, transfer_A_to_B, NULL);
    pthread_create(&t2, NULL, transfer_B_to_A, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("No deadlock occurred.\n"); // Output displays only if no deadlocking occurs
    return 0;
}

