#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int start;
    int end;
    double result;
} ThreadData;

double max_likelihood_sequential(int n);
void *max_likelihood_parallel(void *arg);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <Number of threads> <Number of samples>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    int num_samples = atoi(argv[2]);
    if (num_threads <= 0 || num_samples <= 0)
    {
        printf("Wrong input parameters! Number of the threads and the samples must be a positive value!\n");
        return 1;
    }

    clock_t start, end;
    double sequential_time, parallel_time, sequential_result, parallel_result;

    start = clock();
    sequential_result = max_likelihood_sequential(num_samples);
    end = clock();
    sequential_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    int samples_per_thread = num_samples / num_threads;
    int i;

    start = clock();
    for (i = 0; i < num_threads; i++)
    {
        thread_data[i].start = i * samples_per_thread;
        thread_data[i].end = (i + 1) * samples_per_thread;
        pthread_create(&threads[i], NULL, max_likelihood_parallel, (void *)&thread_data[i]);
    }

    parallel_result = 0;
    for (i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
        parallel_result += thread_data[i].result;
    }
    end = clock();
    parallel_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Sequential result: %f\n", sequential_result);
    printf("Parallel result: %f\n", parallel_result);
    printf("Sequential time: %f sec\n", sequential_time);
    printf("Parallel time: %f sec\n", parallel_time);

    return 0;
}

double max_likelihood_sequential(int n) {
    double result = 0;
    int i;
    for (i = 0; i < n; i++)
        result += i;

    return result;
}

void *max_likelihood_parallel(void *arg) 
{
    ThreadData *data = (ThreadData *)arg;
    double result = 0;
    int i;
    for (i = data->start; i < data->end; i++)
        result += i;

    data->result = result;
    return NULL;
}