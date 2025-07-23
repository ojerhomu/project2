/*
This is the file for the resource_protection demo. The features a 
simulation which creates multiple threads each of which will 
perform a thousand deposit and withdrawal operations. This is a
race-free implementation which uses a mutually exclusive function
with a lock variable.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4
#define NUM_OPS 1000

typedef struct {
    int balance;
    pthread_mutex_t lock; // creation of mutex "lock" 
} Account;

Account account = { .balance = 0, .lock = PTHREAD_MUTEX_INITIALIZER };

void* deposit_and_withdraw(void* arg) { // mutex function which implements a lock feature that is used by each thread
    for (int i = 0; i < NUM_OPS; i++) {
        // Deposit
        pthread_mutex_lock(&account.lock);
        account.balance += 1;           
        pthread_mutex_unlock(&account.lock);

        // Withdraw
        pthread_mutex_lock(&account.lock);
        account.balance -= 1; 
        pthread_mutex_unlock(&account.lock);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, deposit_and_withdraw, NULL);  // Creates and starts multiple threads which will each utilize the mutex feature. 

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL); // Threads wait on each other to end

    printf("Final balance : %d\n", account.balance); // A final output of "0" would indicate that the program ran cleanly without any data conflicts
    return 0;
}

