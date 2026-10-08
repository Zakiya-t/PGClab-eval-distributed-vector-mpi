# Benchmark Output Evidence

These excerpts summarize the measured runs used to populate `benchmark_measurements.csv`.

## Sequential

```text
N=600   Dot Product=600.00   Time=0.000000769 s   PASSED
N=1200  Dot Product=1200.00  Time=0.000001483 s   PASSED
N=1800  Dot Product=1800.00  Time=0.000001972 s   PASSED
N=2400  Dot Product=2400.00  Time=0.000002570 s   PASSED
N=3000  Dot Product=3000.00  Time=0.000003204 s   PASSED
```

## MPI

```text
N=600   200 elements/process   Time=0.001921705 s   Dot=600.00   PASSED
N=1200  400 elements/process   Time=0.002759280 s   Dot=1200.00  PASSED
N=1800  600 elements/process   Time=0.001386273 s   Dot=1800.00  PASSED
N=2400  800 elements/process   Time=0.002179073 s   Dot=2400.00  PASSED
N=3000  1000 elements/process  Time=0.001537805 s   Dot=3000.00  PASSED
```
