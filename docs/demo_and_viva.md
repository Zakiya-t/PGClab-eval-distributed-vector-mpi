# Demonstration and Viva Guide

## Demonstration Order

1. Show Master, Worker 1 and Worker 2 VMs.
2. Show hostnames and IP addresses.
3. Demonstrate `ping` connectivity.
4. Demonstrate passwordless SSH from Master.
5. Show `mpicc --version` and `mpirun --version`.
6. Run the MPI communication test.
7. Run the sequential dot product.
8. Show the sequential correctness result.
9. Deploy the MPI executable to the Workers.
10. Run the distributed MPI dot product.
11. Show each rank and hostname.
12. Verify the final dot product.
13. Show the performance table.
14. Explain speedup and parallel efficiency.
15. Show the three performance graphs.

## Viva Questions

### What is MPI?
MPI is a message-passing standard used to communicate between independent processes in parallel/distributed programs.

### What is a rank?
A rank is the unique identifier assigned to an MPI process within a communicator.

### Why are three VMs used?
The three VMs simulate a small distributed-memory cluster.

### What does MPI_Scatter do?
It distributes equal portions of a buffer from the root process to all participating processes.

### What does MPI_Reduce do?
It combines values from all processes using an operation such as `MPI_SUM` and returns the combined result to the root.

### Why is MPI_SUM used here?
Each process computes a partial dot product. The final dot product is the sum of those partial dot products.

### Why might MPI be slower?
For a small vector, computation is tiny but MPI still incurs communication, synchronization and virtual-machine/network overhead.

### How do you know the MPI program is truly distributed?
The communication test and vector program report different ranks executing on `master`, `worker1`, and `worker2`.
