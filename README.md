# Distributed Vector Dot Product using MPI

A 3-node distributed-memory parallel computing laboratory project implementing vector dot product with **MPI (Message Passing Interface)** and comparing it against a sequential CPU implementation.

> **Course problem statement:** Distributed Vector Processing – MPI – Divide a large vector among processes and perform computations.

## Project Overview

This project implements the dot product of two vectors using:

1. **Sequential processing** on the Master VM.
2. **Distributed MPI processing** across three Ubuntu VMs.

For the distributed version, Rank 0 (Master) initializes the vectors, `MPI_Scatter` distributes vector portions to all three ranks, each rank computes its local dot product, and `MPI_Reduce` with `MPI_SUM` produces the final result on Rank 0.

The program accepts the vector size from the command line, so the same implementation can be used for the required benchmark sizes and for larger scalability experiments.

## Architecture

```text
                         MASTER
                       MPI Rank 0
                    192.168.217.128
                           |
                -----------------------
                |                     |
                v                     v
             WORKER 1              WORKER 2
              Rank 1                Rank 2
          192.168.217.130      192.168.217.131
                |                     |
                -------- MPI ---------
                         |
                   Distributed Work
```

### Responsibilities

**Master / Rank 0**
- Creates and initializes vectors A and B.
- Distributes vector portions.
- Receives the reduced final dot product.
- Reports execution time and correctness.

**Worker 1 / Rank 1** and **Worker 2 / Rank 2**
- Receive local vector portions.
- Compute local dot products independently.
- Contribute partial results to `MPI_Reduce`.

## MPI Computation Flow

```text
Rank 0 creates A and B
        |
        +---- MPI_Scatter(A) ---->
        +---- MPI_Scatter(B) ---->
                    |
          +---------+---------+
          |         |         |
        Rank 0    Rank 1    Rank 2
          |         |         |
       local dot local dot local dot
          \         |         /
           \        |        /
              MPI_Reduce
                MPI_SUM
                   |
                   v
              Final dot product
                 Rank 0
```

For a vector size `N` and three MPI processes:

```text
Elements per process = N / 3
```

The official benchmark sizes therefore distribute as:

| Vector size | Rank 0 | Rank 1 | Rank 2 |
|---:|---:|---:|---:|
| 600 | 200 | 200 | 200 |
| 1200 | 400 | 400 | 400 |
| 1800 | 600 | 600 | 600 |
| 2400 | 800 | 800 | 800 |
| 3000 | 1000 | 1000 | 1000 |

## Correctness

For the official benchmark, both vectors are initialized with `1.0`, so:

```text
Dot(A, B) = 1 + 1 + ... + 1 = N
```

Every official benchmark run was verified successfully:

| Vector size | Expected | Sequential | MPI |
|---:|---:|---:|---:|
| 600 | 600 | PASSED | PASSED |
| 1200 | 1200 | PASSED | PASSED |
| 1800 | 1800 | PASSED | PASSED |
| 2400 | 2400 | PASSED | PASSED |
| 3000 | 3000 | PASSED | PASSED |

An additional small-vector test can be used to demonstrate that the program performs the actual multiplication and accumulation rather than simply returning `N`:

```text
A = [1, 2, 3, 4]
B = [2, 3, 4, 5]
Dot Product = 40
```

## Actual Performance Results

The values below are the **actual measured results from the laboratory run**. They were not manually invented.

| Vector Size | Sequential (s) | MPI (s) | Elements / Process | Speedup | Efficiency |
|---:|---:|---:|---:|---:|---:|
| 600 | 0.000000769 | 0.001921705 | 200 | 0.000400165x | 0.013339% |
| 1200 | 0.000001483 | 0.002759280 | 400 | 0.000537459x | 0.017915% |
| 1800 | 0.000001972 | 0.001386273 | 600 | 0.001422519x | 0.047417% |
| 2400 | 0.000002570 | 0.002179073 | 800 | 0.001179401x | 0.039313% |
| 3000 | 0.000003204 | 0.001537805 | 1000 | 0.002083489x | 0.069450% |

### Performance interpretation

The required benchmark sizes are very small. The dot-product computation itself takes only microseconds on the Master CPU, while the MPI version also pays for process coordination, data distribution, synchronization, and network communication between virtual machines. Consequently, MPI is slower for these particular small workloads.

This is a **workload-size effect, not a correctness failure**. The project demonstrates true distributed execution and is designed to scale to larger vector sizes. Larger-vector runs should be treated as separate scalability experiments and recorded only after obtaining actual measurements.

## Performance Formulas

### Speedup

```text
Speedup = Sequential Time / MPI Time
```

### Parallel Efficiency

```text
Efficiency = Speedup / Number of MPI Processes × 100
```

For this project:

```text
Number of MPI Processes = 3
```

## Repository Structure

```text
distributed-vector-mpi/
│
├── README.md
├── Makefile
├── .gitignore
│
├── config/
│   └── hosts
│
├── src/
│   ├── mpi_test.c
│   ├── vector_dot_sequential.c
│   └── vector_dot_mpi.c
│
├── scripts/
│   ├── run_sequential.sh
│   ├── run_mpi.sh
│   ├── deploy_mpi.sh
│   └── calculate_metrics.py
│
├── docs/
│   ├── cluster_config.md
│   ├── algorithm.md
│   ├── communication_test.md
│   ├── results.md
│   ├── performance_analysis.md
│   └── demo_and_viva.md
│
├── results/
│   ├── raw/
│   │   └── benchmark_measurements.csv
│   ├── processed/
│   │   └── performance_summary.csv
│   └── graphs/
│       ├── execution_time.png
│       ├── speedup.png
│       └── efficiency.png
│
└── screenshots/
    ├── setup/
    ├── sequential/
    ├── mpi/
    └── results/
```

## Requirements

- Ubuntu Linux on Master, Worker 1 and Worker 2
- GCC
- GNU Make
- OpenSSH Server
- Open MPI (`openmpi-bin`, `libopenmpi-dev`)
- Three VMs connected to the same VMware network
- Passwordless SSH from Master to Workers

## Cluster Configuration Used

| Node | Role | IP address | vCPU | RAM |
|---|---|---|---:|---:|
| Master | Rank 0 | 192.168.217.128 | 4 | 4.8 GiB |
| Worker 1 | Rank 1 | 192.168.217.130 | 4 | 4.8 GiB |
| Worker 2 | Rank 2 | 192.168.217.131 | 4 | 4.8 GiB |

The `172.17.0.1` interface visible on the VMs is the Docker bridge. MPI execution is explicitly forced through the VMware `192.168.217.0/24` network.

## Setup

Install the required packages on all three VMs:

```bash
sudo apt update
sudo apt install openssh-server openmpi-bin libopenmpi-dev gcc make -y
```

Configure hostname resolution so `master`, `worker1`, and `worker2` resolve to the correct VM addresses.

Verify passwordless SSH from Master:

```bash
ssh -o BatchMode=yes worker1 hostname
ssh -o BatchMode=yes worker2 hostname
```

Expected:

```text
worker1
worker2
```

## Build

On the Master:

```bash
cd ~/distributed-vector-mpi
make all
```

The build output is placed under `build/` and is intentionally ignored by Git.

## MPI Deployment

Deploy the compiled MPI executable from Master to the Workers:

```bash
./scripts/deploy_mpi.sh
```

The deployment places the executable at:

```text
/home/zakiya/vector_dot_mpi
```

on each Worker in the current laboratory environment.

## Run the Communication Test

```bash
mpirun -np 3 --hostfile config/hosts build/mpi_test
```

Expected mapping:

```text
Rank 0 → master
Rank 1 → worker1
Rank 2 → worker2
```

## Run Sequential Dot Product

Example:

```bash
./scripts/run_sequential.sh 600
```

## Run Distributed MPI Dot Product

The project uses the following forced-network command through `scripts/run_mpi.sh`:

```bash
./scripts/run_mpi.sh 600
```

Internally it uses:

```bash
env -u DISPLAY mpirun \
  -np 3 \
  --hostfile config/hosts \
  --mca btl self,tcp \
  --mca btl_tcp_if_include 192.168.217.0/24 \
  --mca oob_tcp_if_include 192.168.217.0/24 \
  build/vector_dot_mpi 600
```

## Required Benchmark

Run both implementations for:

```text
600
1200
1800
2400
3000
```

Record the actual execution times in:

```text
results/raw/benchmark_measurements.csv
```

Then regenerate metrics and graphs:

```bash
python3 scripts/calculate_metrics.py
```

This produces:

- `results/processed/performance_summary.csv`
- `results/graphs/execution_time.png`
- `results/graphs/speedup.png`
- `results/graphs/efficiency.png`

## Large-Vector Scalability

The programs accept arbitrary positive vector sizes that are divisible by three. For example:

```bash
./scripts/run_sequential.sh 1000000
./scripts/run_mpi.sh 1000000
```

A larger-vector result should be added to the repository only after it has been measured experimentally on the three-VM cluster.

## Evidence / Screenshots

Place only useful evaluation evidence in the `screenshots/` folders.

### Setup
- Three VM cluster view
- Master/Worker hostnames
- IP configuration
- Ping connectivity
- Passwordless SSH
- MPI version verification

### Sequential
- Compilation
- Correctness output
- Execution-time output

### MPI
- MPI communication test
- Compilation
- Distributed rank/hostname output
- Correctness output
- Execution time

### Results
- Final performance table
- Execution-time graph
- Speedup graph
- Efficiency graph

## Evaluation Flow

```text
1. Show the three VMs
2. Show hostname/IP configuration
3. Show connectivity
4. Show passwordless SSH
5. Show MPI installation
6. Run MPI communication test
7. Run sequential dot product
8. Verify sequential result
9. Run distributed MPI dot product
10. Verify distributed result
11. Compare execution times
12. Explain speedup and efficiency
13. Show performance graphs
```

## Key Takeaway

This project demonstrates the complete distributed-processing workflow: a vector computation is divided across independent MPI processes on separate VMs, local work is performed independently, and partial results are combined into one final answer. The measured benchmark results also illustrate an important principle of parallel computing: communication overhead can dominate when the computation is too small, while the same distributed implementation can be reused for substantially larger workloads.
