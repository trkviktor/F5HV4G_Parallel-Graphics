// Floyd-Warshall algorithm

// Import header files for programy
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <Number of threads> <Number of N>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    int N = atoi(argv[2]);
    int nthreads;
    int src, dst, middle;
    int **distance_matrix;

    distance_matrix = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
    {
        distance_matrix[i] = (int *)malloc(N * sizeof(int));
    }

    for (src = 0; src < N; src++)
    {
        for (dst = 0; dst < N; dst++)
        {
            if (src != dst)
            {
                distance_matrix[src][dst] = rand() % 20;
            }
        }
    }

    double start_time = omp_get_wtime();

    for (middle = 0; middle < N; middle++)
    {
        int *dm = distance_matrix[middle];
        for (src = 0; src < N; src++)
        {
            int *ds = distance_matrix[src];
            for (dst = 0; dst < N; dst++)
            {
                ds[dst] = min(ds[dst], ds[middle] + dm[dst]);
            }
        }
    }

    double time = omp_get_wtime() - start_time;
    printf("Total time for sequential (in sec):%.2f\n", time);

    for (nthreads = 1; nthreads <= num_threads; nthreads++)
    {
        omp_set_num_threads(nthreads);

        double start_time = omp_get_wtime();

#pragma omp parallel shared(distance_matrix)
        for (middle = 0; middle < N; middle++)
        {
            int *dm = distance_matrix[middle];
#pragma omp parallel for private(src, dst) schedule(dynamic)
            for (src = 0; src < N; src++)
            {
                int *ds = distance_matrix[src];
                for (dst = 0; dst < N; dst++)
                {
                    ds[dst] = min(ds[dst], ds[middle] + dm[dst]);
                }
            }
        }

        double time = omp_get_wtime() - start_time;
        printf("Total time for thread %d (in sec):%.2f\n", nthreads, time);
    }
    return 0;
}