#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double get_time_seconds(void)
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
    {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

int main(int argc, char *argv[])
{
    int N;
    double *A = NULL;
    double *B = NULL;
    double dot = 0.0;
    double start, end;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <vector_size>\n", argv[0]);
        return EXIT_FAILURE;
    }

    N = atoi(argv[1]);
    if (N <= 0)
    {
        fprintf(stderr, "Error: vector size must be positive.\n");
        return EXIT_FAILURE;
    }

    A = malloc((size_t)N * sizeof(*A));
    B = malloc((size_t)N * sizeof(*B));

    if (A == NULL || B == NULL)
    {
        fprintf(stderr, "Error: memory allocation failed.\n");
        free(A);
        free(B);
        return EXIT_FAILURE;
    }

    /* Main experiment: both vectors contain 1.0. */
    for (int i = 0; i < N; ++i)
    {
        A[i] = 1.0;
        B[i] = 1.0;
    }

    start = get_time_seconds();

    for (int i = 0; i < N; ++i)
    {
        dot += A[i] * B[i];
    }

    end = get_time_seconds();

    printf("\nSequential Vector Dot Product\n");
    printf("Vector Size = %d\n", N);
    printf("Dot Product = %.2f\n", dot);
    printf("Execution Time = %.9f seconds\n", end - start);
    printf("Verification = %s\n",
           (dot == (double)N) ? "PASSED" : "FAILED");

    free(A);
    free(B);
    return EXIT_SUCCESS;
}
