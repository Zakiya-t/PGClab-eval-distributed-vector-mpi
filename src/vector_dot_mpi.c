#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int N;
    int elements_per_process;
    char hostname[256];

    double *A = NULL;
    double *B = NULL;
    double *local_A = NULL;
    double *local_B = NULL;

    double local_dot = 0.0;
    double global_dot = 0.0;
    double start, end;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    gethostname(hostname, sizeof(hostname));

    if (argc != 2)
    {
        if (rank == 0)
            fprintf(stderr, "Usage: %s <vector_size>\n", argv[0]);

        MPI_Finalize();
        return EXIT_FAILURE;
    }

    N = atoi(argv[1]);

    if (N <= 0)
    {
        if (rank == 0)
            fprintf(stderr, "Error: vector size must be positive.\n");

        MPI_Finalize();
        return EXIT_FAILURE;
    }

    /* The evaluation architecture uses exactly 3 MPI processes. */
    if (size != 3)
    {
        if (rank == 0)
            fprintf(stderr,
                    "Error: this experiment requires exactly 3 MPI processes.\n");

        MPI_Finalize();
        return EXIT_FAILURE;
    }

    /* The official benchmark sizes divide evenly among 3 processes. */
    if (N % size != 0)
    {
        if (rank == 0)
        {
            fprintf(stderr,
                    "Error: vector size %d is not divisible by %d processes.\n",
                    N, size);
        }

        MPI_Finalize();
        return EXIT_FAILURE;
    }

    elements_per_process = N / size;

    local_A = malloc((size_t)elements_per_process * sizeof(*local_A));
    local_B = malloc((size_t)elements_per_process * sizeof(*local_B));

    if (local_A == NULL || local_B == NULL)
    {
        fprintf(stderr,
                "Rank %d on %s: local memory allocation failed.\n",
                rank, hostname);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    /* Rank 0 owns and initializes the full input vectors. */
    if (rank == 0)
    {
        A = malloc((size_t)N * sizeof(*A));
        B = malloc((size_t)N * sizeof(*B));

        if (A == NULL || B == NULL)
        {
            fprintf(stderr, "Rank 0: global memory allocation failed.\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        for (int i = 0; i < N; ++i)
        {
            A[i] = 1.0;
            B[i] = 1.0;
        }

        printf("\nDistributed Vector Dot Product using MPI\n");
        printf("Vector Size = %d\n", N);
        printf("MPI Processes = %d\n", size);
        printf("Elements per Process = %d\n\n", elements_per_process);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    /* Divide vector A equally across all ranks. */
    MPI_Scatter(A,
                elements_per_process,
                MPI_DOUBLE,
                local_A,
                elements_per_process,
                MPI_DOUBLE,
                0,
                MPI_COMM_WORLD);

    /* Divide vector B equally across all ranks. */
    MPI_Scatter(B,
                elements_per_process,
                MPI_DOUBLE,
                local_B,
                elements_per_process,
                MPI_DOUBLE,
                0,
                MPI_COMM_WORLD);

    printf("Rank %d on %s processing %d elements\n",
           rank, hostname, elements_per_process);

    /* Each process computes the dot product of its local portion. */
    for (int i = 0; i < elements_per_process; ++i)
    {
        local_dot += local_A[i] * local_B[i];
    }

    /* Add all local dot products at Rank 0. */
    MPI_Reduce(&local_dot,
               &global_dot,
               1,
               MPI_DOUBLE,
               MPI_SUM,
               0,
               MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0)
    {
        printf("\nMPI Computation Completed\n");
        printf("Vector Size = %d\n", N);
        printf("Number of MPI Processes = %d\n", size);
        printf("Execution Time = %.9f seconds\n", end - start);
        printf("Dot Product = %.2f\n", global_dot);
        printf("Verification = %s\n",
               (global_dot == (double)N) ? "PASSED" : "FAILED");
    }

    free(local_A);
    free(local_B);

    if (rank == 0)
    {
        free(A);
        free(B);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
