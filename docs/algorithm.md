# Algorithm

## Sequential Algorithm

1. Read vector size `N` from the command line.
2. Allocate vectors `A` and `B`.
3. Initialize every element of both vectors to `1.0`.
4. Compute:
   ```text
   dot = Σ A[i] × B[i]
   ```
5. Measure the computation time using a monotonic wall-clock timer.
6. Display the result and verify it against `N`.
7. Free allocated memory.

## MPI Algorithm

1. Initialize MPI.
2. Get the process rank and total number of processes.
3. Require exactly three MPI processes for this evaluation.
4. Read `N` and verify that `N % 3 == 0`.
5. Rank 0 allocates and initializes the complete vectors A and B.
6. Allocate local vector portions on every process.
7. Synchronize with `MPI_Barrier` and start the MPI timer.
8. Use `MPI_Scatter` to distribute portions of A.
9. Use `MPI_Scatter` to distribute portions of B.
10. Each rank computes its local dot product.
11. Use `MPI_Reduce` with `MPI_SUM` to combine all local dot products at Rank 0.
12. Stop the timer on every rank.
13. Rank 0 prints the final result, timing and verification status.
14. Free memory and finalize MPI.

## Example: N = 600

With 3 processes:

```text
600 / 3 = 200 elements per process
```

Each process calculates 200 products, and Rank 0 receives:

```text
local_dot(rank 0) + local_dot(rank 1) + local_dot(rank 2)
```

Because all input values are `1.0`, the result is `600.0`.
