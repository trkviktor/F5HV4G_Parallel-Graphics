#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void *calc(void* data){
    printf("%d. thread calc started\n",(int*)data);
    int msec = 0;
    clock_t before = clock();
    int sum = 0;
    do {
        sum += rand();
        //printf("first thread: %d \n",sum);

    clock_t difference = clock() - before;
    msec = difference * 1000 / CLOCKS_PER_SEC;
    } while ( msec < 1000 );
    printf("%d. thread took %d seconds, sum = %d \n",(int*)data,msec/1000,sum);
}

int main(){
    srand(time(NULL));

    clock_t before = clock();
    pthread_t threadArray[60];
    for (int i = 0; i < 60; i++){
        pthread_t temp;
        threadArray[i] = pthread_create(&temp, NULL, calc, i);
        
    }
    do {
        
    }while(pthread_kill(&threadArray[59], 0) == 0);
    clock_t difference = clock() - before;
    printf("all threads done after: %d\n",difference);
    return 0;
}