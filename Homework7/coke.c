#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>

/************************************************************************
 * preprocessor directives
 ************************************************************************/
#define INITIAL_NUMBER_COKES 5
#define MAX_NUMBER_COKES 10

#define MAX_RUNS 6

#define NUMBER_PRODUCERS 10
#define NUMBER_CONSUMERS 3


/************************************************************************
 * for convenience, global variables - to be read by threads
 ************************************************************************/
pthread_mutex_t lock;
pthread_cond_t not_empty, not_full;
int cokes = INITIAL_NUMBER_COKES;


/************************************************************************
 * function prototype declarations
 ************************************************************************/
void* thread_function(void* function);

void refill_coke(void);
void consume_coke(void);


/************************************************************************
 * MAIN
 ************************************************************************/
int main(int argc, const char * argv[]) {

    int i;
    pthread_t producers[NUMBER_PRODUCERS];
    pthread_t consumers[NUMBER_CONSUMERS];

    // init mutex and condition variables, see above
    // ... FILL IN
    lock = PTHREAD_MUTEX_INITIALIZER;
    not_empty = PTHREAD_COND_INITIALIZER;
    not_full = PTHREAD_COND_INITIALIZER;

    // create consumers
    for (i=0; i<NUMBER_CONSUMERS; i++) {
        if (pthread_create(&consumers[i],NULL,thread_function,&consume_coke) != 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }

    // create producers
    for (i=0; i<NUMBER_PRODUCERS; i++) {
        if (pthread_create(&producers[i],NULL,thread_function,&refill_coke) != 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }

    // just sleep and then bail out
    sleep(10);

    return EXIT_SUCCESS;
}


/************************************************************************
 * generic thread function
 ************************************************************************/
void* thread_function(void* function) {
    int i;

    for (i=0; i<MAX_RUNS; i++) {
        // call function, either of refill_coke() or consume_coke()
        ((void (*)(void))function)();
    }

    pthread_exit(NULL);
}


/************************************************************************
 * refilling and consuming coke functions
 ************************************************************************/
void refill_coke(void) {
    pthread_mutex_lock(&lock); //enter critical section and lock
    // FILL IN
    while(cokes == MAX_NUMBER_COKES){
        printf("PP Machine is full.\n");
        pthread_cond_wait(&not_full, &lock);
    }
    cokes = MAX_NUMBER_COKES;
    printf(">> Cokes refilled.\n");
    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&lock);//exit critical section and unlock
}


void consume_coke(void) {
    pthread_mutex_lock(&lock); //enter critical section and lock
    // FILL IN
    while(cokes == 0){
        printf("CC Machine is empty.\n");
        pthread_cond_wait(&not_empty, &lock);
    }
    cokes = cokes - 1;
    printf("<< Coke removed. %d are left.\n", cokes);
    pthread_cond_signal(&not_full);
    pthread_mutex_unlock(&lock);//exit critical section and unlock
}










