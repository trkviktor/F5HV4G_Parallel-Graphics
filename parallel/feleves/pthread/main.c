#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

typedef struct {
    int start;
    int end;
    double *data;
    double mean;
    double variance;
} ThreadData;

double likelihood(double x, double mean, double variance);
double max_likelihood_sequential(double *data, int n);
void *max_likelihood_parallel(void *arg);

double get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

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

    double *data = malloc(num_samples * sizeof(double));
    if (data == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Generate some random data
    srand(time(NULL));
    for (int i = 0; i < num_samples; i++)
    {
        data[i] = rand() / (double)RAND_MAX;
    }

    double sequential_result, parallel_result;
    double sequential_time, parallel_time;

    double start_time = get_time();
    sequential_result = max_likelihood_sequential(data, num_samples);
    double end_time = get_time();
    sequential_time = end_time - start_time;

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    int samples_per_thread = num_samples / num_threads;

    start_time = get_time();
    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].start = i * samples_per_thread;
        thread_data[i].end = (i + 1) * samples_per_thread;
        thread_data[i].data = data;
        pthread_create(&threads[i], NULL, max_likelihood_parallel, (void *)&thread_data[i]);
    }

    parallel_result = 0;
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
        parallel_result += thread_data[i].mean;
    }
    end_time = get_time();
    parallel_time = end_time - start_time;

    printf("Sequential result: mean=%.4f variance=%.4f\n", sequential_result, 1 / (2 * sequential_result));
    printf("Parallel result: mean=%.4f variance=%.4f\n", parallel_result / num_threads, 1 / (2 * parallel_result / num_threads));
    printf("Sequential time: %.6f sec\n", sequential_time);
    printf("Parallel time: %.6f sec\n", parallel_time);

    free(data);

    return 0;
}

double likelihood(double x, double mean, double variance)
{
    double exponent = -0.5 * pow((x - mean), 2) / variance;
    double coef = 1.0 / sqrt(2 * M_PI * variance);
    return coef * exp(exponent);
}

double max_likelihood_sequential(double *data, int n)
{
    double sum = 0;
    for (int i = 0; i < n; i++)
        sum += data[i];

    return sum / n;
}

void *max_likelihood_parallel(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    double sum = 0;
    for (int i = data->start; i < data->end; i++)
        sum += data->data[i];

    data->mean = sum / (data->end - data->start);
    data->variance = 0;

    for (int i = data->start; i < data->end; i++)
        data->variance += pow(data->data[i] - data->mean, 2);

    data->variance /= (data->end - data->start);

    return NULL;
}
