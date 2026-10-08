#!/usr/bin/env bash
set -euo pipefail

N="${1:-}"
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
HOSTFILE="$ROOT_DIR/config/hosts"
EXECUTABLE="$HOME/vector_dot_mpi"

if [[ -z "$N" ]]; then
    echo "Usage: $0 <vector_size>"
    exit 1
fi

if [[ ! -x "$EXECUTABLE" ]]; then
    echo "MPI executable not found at $EXECUTABLE."
    echo "Run: make mpi && ./scripts/deploy_mpi.sh"
    exit 1
fi

# Force Open MPI to use the VMware 192.168.217.x network instead of Docker's 172.17.x bridge.
exec env -u DISPLAY mpirun \
    -np 3 \
    --hostfile "$HOSTFILE" \
    --mca btl self,tcp \
    --mca btl_tcp_if_include 192.168.217.0/24 \
    --mca oob_tcp_if_include 192.168.217.0/24 \
    "$EXECUTABLE" "$N"
