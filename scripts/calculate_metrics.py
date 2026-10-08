#!/usr/bin/env python3
"""Calculate speedup/efficiency and generate the required performance graphs."""
from __future__ import annotations

import csv
from pathlib import Path

import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parents[1]
RAW = ROOT / "results" / "raw" / "benchmark_measurements.csv"
PROCESSED = ROOT / "results" / "processed" / "performance_summary.csv"
GRAPHS = ROOT / "results" / "graphs"
PROCESSED.parent.mkdir(parents=True, exist_ok=True)
GRAPHS.mkdir(parents=True, exist_ok=True)

PROCESSES = 3

rows = []
with RAW.open(newline="", encoding="utf-8") as f:
    for row in csv.DictReader(f):
        n = int(row["vector_size"])
        seq = float(row["sequential_time_s"])
        mpi = float(row["mpi_time_s"])
        speedup = seq / mpi if mpi > 0 else 0.0
        efficiency = speedup / PROCESSES * 100.0
        rows.append(
            {
                **row,
                "speedup": f"{speedup:.9f}",
                "parallel_efficiency_percent": f"{efficiency:.9f}",
            }
        )

with PROCESSED.open("w", newline="", encoding="utf-8") as f:
    fieldnames = [
        "vector_size",
        "sequential_time_s",
        "mpi_time_s",
        "elements_per_process",
        "expected_dot_product",
        "sequential_verification",
        "mpi_verification",
        "speedup",
        "parallel_efficiency_percent",
    ]
    writer = csv.DictWriter(f, fieldnames=fieldnames)
    writer.writeheader()
    writer.writerows(rows)

sizes = [int(r["vector_size"]) for r in rows]
seq_times = [float(r["sequential_time_s"]) for r in rows]
mpi_times = [float(r["mpi_time_s"]) for r in rows]
speedups = [float(r["speedup"]) for r in rows]
eff = [float(r["parallel_efficiency_percent"]) for r in rows]

plt.figure(figsize=(9, 5.5))
plt.plot(sizes, seq_times, marker="o", label="Sequential")
plt.plot(sizes, mpi_times, marker="o", label="MPI (3 processes)")
plt.xlabel("Vector Size")
plt.ylabel("Execution Time (seconds)")
plt.title("Vector Size vs Execution Time")
plt.yscale("log")
plt.grid(True, which="both", linestyle="--", alpha=0.35)
plt.legend()
plt.tight_layout()
plt.savefig(GRAPHS / "execution_time.png", dpi=220)
plt.close()

plt.figure(figsize=(9, 5.5))
plt.plot(sizes, speedups, marker="o")
for x, y in zip(sizes, speedups):
    plt.annotate(f"{y:.2e}", (x, y), xytext=(0, 8), textcoords="offset points", ha="center", fontsize=9)
plt.xlabel("Vector Size")
plt.ylabel("Speedup (×)")
plt.title("Vector Size vs MPI Speedup (log scale)")
plt.yscale("log")
plt.grid(True, which="both", linestyle="--", alpha=0.35)
plt.tight_layout()
plt.savefig(GRAPHS / "speedup.png", dpi=220)
plt.close()

plt.figure(figsize=(9, 5.5))
plt.plot(sizes, eff, marker="o")
for x, y in zip(sizes, eff):
    plt.annotate(f"{y:.2e}%", (x, y), xytext=(0, 8), textcoords="offset points", ha="center", fontsize=9)
plt.xlabel("Vector Size")
plt.ylabel("Parallel Efficiency (%)")
plt.title("Vector Size vs Parallel Efficiency (log scale)")
plt.yscale("log")
plt.grid(True, which="both", linestyle="--", alpha=0.35)
plt.tight_layout()
plt.savefig(GRAPHS / "efficiency.png", dpi=220)
plt.close()

print(f"Processed {len(rows)} benchmark rows.")
print(f"Wrote: {PROCESSED}")
print(f"Graphs: {GRAPHS}")
