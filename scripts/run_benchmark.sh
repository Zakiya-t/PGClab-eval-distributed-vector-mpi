#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT_DIR="$ROOT_DIR/results/raw/runtime_logs"
mkdir -p "$OUT_DIR"

for N in 600 1200 1800 2400 3000; do
    echo "===== Sequential N=$N ====="
    "$ROOT_DIR/scripts/run_sequential.sh" "$N" | tee "$OUT_DIR/sequential_${N}.txt"
    echo
    echo "===== MPI N=$N ====="
    "$ROOT_DIR/scripts/run_mpi.sh" "$N" | tee "$OUT_DIR/mpi_${N}.txt"
    echo
 done

 echo "Benchmark run complete. Review the logs before updating the canonical CSV."
