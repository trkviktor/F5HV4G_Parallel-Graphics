#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void *calc(void* data){
    printf("Thread calc started\n");
    int msec = 0;
    clock_t before = clock();
    int sum = 0;
    do {
        sum += rand();
        //printf("first thread: %d \n",sum);

    clock_t difference = clock() - before;
    msec = difference * 1000 / CLOCKS_PER_SEC;
    } while ( msec < (int*)data );
    printf("First thread took %d seconds, sum = %d \n",msec/1000,sum);
}

int main(){
    srand(time(NULL));

    pthread_t th1;

    int r = 0;
    int threadTime = 4000;
    int mainTime = 8000;
    int msec = 0;
    int sum = 0;

    pthread_create(&th1, NULL, calc, threadTime);

    printf("Main calc started\n");
    clock_t before = clock();

    do {
        sum += rand();

    clock_t difference = clock() - before;
    msec = difference * 1000 / CLOCKS_PER_SEC;
    } while ( msec < mainTime );
    printf("First thread took %d seconds, sum = %d \n",msec/1000,sum);
    return 0;
}