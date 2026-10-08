# Results Directory

- `raw/benchmark_measurements.csv` contains the actual measured benchmark values used in the submitted performance comparison.
- `processed/performance_summary.csv` contains derived speedup and parallel-efficiency values.
- `graphs/` contains the three required performance plots.

If new measurements are performed, update the raw CSV first and then regenerate the processed results and graphs with:

```bash
python3 scripts/calculate_metrics.py
```
