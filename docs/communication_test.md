# MPI Communication Test

Before running the vector program, the cluster was verified with a small MPI test program.

## Purpose

The test proves that MPI can launch three ranks across the three VMs and identifies the hostname associated with each rank.

## Program

`src/mpi_test.c` obtains:

- MPI rank
- Number of MPI processes
- Hostname

and prints the mapping.

## Expected Mapping

```text
Rank 0 → master
Rank 1 → worker1
Rank 2 → worker2
```

## Run

```bash
mpirun -np 3 --hostfile config/hosts build/mpi_test
```

The order of printed lines may vary because ranks write to stdout independently. The rank-to-hostname mapping is the important correctness check.
