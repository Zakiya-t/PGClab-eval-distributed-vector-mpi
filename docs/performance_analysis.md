# Performance Analysis

## Metrics

For three MPI processes:

```text
Speedup = T_sequential / T_MPI

Efficiency = Speedup / 3 × 100
```

## Calculated Results

| N | Speedup | Efficiency |
|---:|---:|---:|
| 600 | 0.000400165× | 0.013339% |
| 1200 | 0.000537459× | 0.017915% |
| 1800 | 0.001422519× | 0.047417% |
| 2400 | 0.001179401× | 0.039313% |
| 3000 | 0.002083489× | 0.069450% |

## Interpretation

The MPI implementation is functionally correct but does not outperform the sequential version for the mandated vector sizes. The reason is workload scale: the sequential dot product is completed in only a few microseconds, whereas distributed execution adds MPI initialization/coordination, vector distribution, synchronization, reduction, and VM/network overhead.

This result is useful from a parallel-computing perspective because it demonstrates that parallelism is not automatically faster for every workload. The distributed implementation itself is genuine and scalable: data is actually divided among three processes, each process performs local computation, and the partial results are combined with `MPI_Reduce`.

## How to Present This in the Evaluation

Use the following message:

> MPI successfully distributes the vector computation across three independent processes on three virtual machines. For the small mandated benchmark sizes, communication and synchronization overhead dominates the very small dot-product workload. The implementation is designed to scale to larger vectors, where the computation-to-communication ratio can be more favorable.

Do not replace the measured benchmark results with invented values. Any large-vector performance claim should be supported by a new experimental run.
