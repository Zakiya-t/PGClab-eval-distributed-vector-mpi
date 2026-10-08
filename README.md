Distributed Vector Processing using MPI
Parallel Computing Laboratory – Experiment 5
Problem Statement: Distributed Vector Processing – MPI – Divide a large vector among processes and perform computations.

1. Problem Statement
Given two vectors A and B of size N, calculate their dot product using:
Dot(A, B) = Σ A[i] × B[i]
The MPI implementation must divide the vectors among three MPI processes. Each process computes the dot product of its local portion, and the partial results are combined to obtain the final dot product.
For the official benchmark, both vectors are initialized with 1.0, therefore:
Expected Dot Product = N
The MPI result must match the sequential result for every tested vector size.
2. Objectives
1. Understand distributed vector processing using MPI.
2. Implement vector dot product sequentially.
3. Implement the same computation using MPI.
4. Understand MPI processes and ranks.
5. Distribute vector data using MPI_Scatter.
6. Perform local dot-product computations independently.
7. Combine partial results using MPI_Reduce with MPI_SUM.
8. Compare sequential and MPI execution times.
9. Calculate speedup and parallel efficiency.
10. Analyze the effect of distributed communication and VM overhead on performance.
3. Configuration
3.1 Software
- Ubuntu Linux
- GCC
- GNU Make
- OpenSSH Server
- Open MPI
- VMware Workstation / equivalent virtualization platform
Install the required packages on all three VMs:
sudo apt update
sudo apt install openssh-server openmpi-bin libopenmpi-dev gcc make -y
3.2 Cluster Configuration
Node	Role	MPI Rank	IP Address	vCPU	RAM
Master	Master	0	192.168.217.128	4	4.8 GiB
Worker 1	Worker	1	192.168.217.130	4	4.8 GiB
Worker 2	Worker	2	192.168.217.131	4	4.8 GiB


All three VMs are connected through the same VMware network.
The VMs also expose a Docker bridge interface at 172.17.0.1. MPI execution is explicitly configured to use the VMware network 192.168.217.0/24.
3.3 Hostfile
config/hosts contains:
master slots=1
worker1 slots=1
worker2 slots=1
This configures three MPI execution slots.
3.4 Architecture
                         MASTER
                       MPI Rank 0
                    192.168.217.128
                           |
                -----------------------
                |                     |
                v                     v
             WORKER 1              WORKER 2
              MPI Rank 1            MPI Rank 2
          192.168.217.130      192.168.217.131
3.5 Process Responsibilities
Master / Rank 0
- Creates and initializes vectors A and B.
- Distributes vector portions using MPI.
- Receives the final reduced result.
- Displays execution time and verification status.
Worker 1 / Rank 1
- Receives its local portions of A and B.
- Computes its local dot product.
- Contributes the partial result to MPI_Reduce.
Worker 2 / Rank 2
- Receives its local portions of A and B.
- Computes its local dot product.
- Contributes the partial result to MPI_Reduce.
4. MPI Computation Flow
               Rank 0 creates A and B
                         |
              -----------------------
              |                     |
        MPI_Scatter(A)        MPI_Scatter(B)
              |                     |
              -----------+-----------
                         |
              +----------+----------+
              |          |          |
            Rank 0     Rank 1     Rank 2
              |          |          |
          Local Dot   Local Dot   Local Dot
              \          |          /
               \         |         /
                  MPI_Reduce
                    MPI_SUM
                       |
                       v
                Final Dot Product
                    Rank 0
For three processes:
Elements per process = N / 3
Vector Size	Rank 0	Rank 1	Rank 2
600	200	200	200
1200	400	400	400
1800	600	600	600
2400	800	800	800
3000	1000	1000	1000


5. Execution Steps
5.1 Step 1 – Verify the VMs
On each VM:
hostname
hostname -I
nproc
free -h
Expected hostnames:
master
worker1
worker2
5.2 Step 2 – Configure Network Resolution
Add the following mappings to /etc/hosts on all three VMs:
192.168.217.128 master
192.168.217.130 worker1
192.168.217.131 worker2
Verify from Master:
ping -c 4 worker1
ping -c 4 worker2
The cluster produced successful connectivity with 0% packet loss.
5.3 Step 3 – Configure Passwordless SSH
On Master:
ssh-keygen -t rsa
ssh-copy-id worker1
ssh-copy-id worker2
Verify:
ssh -o BatchMode=yes worker1 hostname
ssh -o BatchMode=yes worker2 hostname
Expected:
worker1
worker2
5.4 Step 4 – Verify MPI Communication
Create and compile the communication test:
mpicc src/mpi_test.c -o src/mpi_test
Copy the executable to both workers:
scp src/mpi_test worker1:~/mpi_test
scp src/mpi_test worker2:~/mpi_test
Run from Master:
env -u DISPLAY mpirun -np 3 --hostfile config/hosts /home/zakiya/mpi_test
The MPI communication test successfully verified:
Rank 0 of 3 running on master
Rank 1 of 3 running on worker1
Rank 2 of 3 running on worker2
5.5 Step 5 – Build the Sequential Program
gcc -O2 src/vector_dot_sequential.c -o vector_dot_sequential
Run using:
./vector_dot_sequential 600
5.6 Step 6 – Build the MPI Program
Compile on Master:
mpicc -O2 src/vector_dot_mpi.c -o vector_dot_mpi
Copy the executable to both workers:
scp ~/distributed-vector-mpi/vector_dot_mpi worker1:~/vector_dot_mpi
scp ~/distributed-vector-mpi/vector_dot_mpi worker2:~/vector_dot_mpi
5.7 Step 7 – Run the MPI Program
The final MPI execution uses the VMware network explicitly:
env -u DISPLAY mpirun -np 3 \
--hostfile ~/distributed-vector-mpi/config/hosts \
--mca btl self,tcp \
--mca btl_tcp_if_include 192.168.217.0/24 \
--mca oob_tcp_if_include 192.168.217.0/24 \
/home/zakiya/vector_dot_mpi 600
Change only the final vector size for the other tests:
1200
1800
2400
3000
5.8 Step 8 – Run the Required Benchmark Sizes
The official benchmark uses:
600, 1200, 1800, 2400, 3000
The same vector size is used for sequential and MPI execution.
6. Results
6.1 Communication Test Result
The three-process MPI communication test successfully mapped the ranks to the intended VMs:
MPI Rank	Host
0	Master
1	Worker 1
2	Worker 2


6.2 Sequential Results
All official sequential tests produced the expected dot product and passed correctness verification.
Vector Size	Dot Product	Execution Time (s)	Verification
600	600.00	0.000000769	PASSED
1200	1200.00	0.000001483	PASSED
1800	1800.00	0.000001972	PASSED
2400	2400.00	0.000002570	PASSED
3000	3000.00	0.000003204	PASSED


6.3 MPI Results
All official MPI tests distributed the workload equally among the three processes and passed correctness verification.
Vector Size	Elements / Process	MPI Time (s)	Dot Product	Verification
600	200	0.001921705	600.00	PASSED
1200	400	0.002759280	1200.00	PASSED
1800	600	0.001386273	1800.00	PASSED
2400	800	0.002179073	2400.00	PASSED
3000	1000	0.001537805	3000.00	PASSED


6.4 Correctness Verification
For the official benchmark, the vectors contain only 1.0 values. Therefore the expected dot product is exactly N.
600  → 600.00  → PASSED
1200 → 1200.00 → PASSED
1800 → 1800.00 → PASSED
2400 → 2400.00 → PASSED
3000 → 3000.00 → PASSED
The program performs the actual multiplication and accumulation:
local_dot += local_A[i] * local_B[i];
and combines the partial results using MPI_Reduce with MPI_SUM.
7. Performance Analysis
7.1 Performance Comparison
The final comparison below uses the actual measured execution times from the laboratory runs.
Vector Size	Sequential (s)	MPI (s)	Speedup	Parallel Efficiency
600	0.000000769	0.001921705	0.000400x	0.0133%
1200	0.000001483	0.002759280	0.000537x	0.0179%
1800	0.000001972	0.001386273	0.001423x	0.0474%
2400	0.000002570	0.002179073	0.001179x	0.0393%
3000	0.000003204	0.001537805	0.002083x	0.0694%


7.2 Speedup Formula
Speedup = Sequential Execution Time / MPI Execution Time
7.3 Parallel Efficiency Formula
For three MPI processes:
Efficiency = (Speedup / 3) × 100
7.4 Performance Interpretation
The MPI implementation demonstrates true distributed-memory processing across three independent virtual machines. The vectors are divided among the three ranks, the local computations are performed independently, and the final result is combined using MPI reduction.
For the specified benchmark sizes, the actual dot-product computation is extremely small. Consequently, MPI communication, synchronization, process-management, and virtual-network overhead dominate the measured execution time. This is why the sequential implementation is faster for these particular small workloads.
This result does not indicate an incorrect MPI implementation. It demonstrates an important distributed-computing principle: parallel processing becomes more useful when the computation is large enough to amortize communication and coordination overhead.
The implementation accepts the vector size as a command-line argument, so the same program can be used for substantially larger vectors without changing the core algorithm.
7.5 Performance Graphs
Execution Time Comparison

Speedup

Parallel Efficiency

7.6 Performance Summary
- Sequential processing provides the lowest overhead for the small official workloads.
- MPI successfully distributes the workload across three VMs, demonstrating distributed-memory execution.
- MPI_Scatter is used to distribute both vectors.
- Each rank performs its local dot-product computation independently.
- MPI_Reduce(MPI_SUM) combines the three partial results.
- The implementation is scalable in input size because N is provided at runtime.
- For larger computational workloads, the fixed communication overhead becomes relatively less significant.
8. Repository Structure
distributed-vector-mpi/
│
├── README.md
├── Makefile
├── .gitignore
│
├── config/
│   ├── hosts
│   └── README.md
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
│       ├── efficiency.png
│       └── README.md
│
└── screenshots/
    ├── setup/
    ├── sequential/
    ├── mpi/
    └── results/
9. Conclusion
This experiment successfully implements Distributed Vector Processing using MPI through a distributed vector dot-product workload.
The experiment establishes a three-node Master–Worker MPI cluster, verifies inter-node communication and passwordless SSH, distributes vector data using MPI_Scatter, performs local dot-product computations on each rank, and combines the partial results using MPI_Reduce(MPI_SUM).
All required vector sizes produced correct results, and the measured performance demonstrates the effect of communication overhead on small distributed workloads. The project also provides a reusable MPI implementation that accepts the vector size at runtime and can be extended to larger computational workloads.
10. Author
Zakiya Tahasildar
Bhavana B H
Pradeep Bichagatti
Abhinandan Belagavi
B.Tech – Computer Science & Artificial Intelligence
KLE Technological University, Hubballi
