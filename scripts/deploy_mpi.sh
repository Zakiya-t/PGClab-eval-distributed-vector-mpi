#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
EXECUTABLE="$ROOT_DIR/build/vector_dot_mpi"
LOCAL_DEPLOY="$HOME/vector_dot_mpi"

if [[ ! -x "$EXECUTABLE" ]]; then
    echo "MPI executable not found. Run: make mpi"
    exit 1
fi

cp "$EXECUTABLE" "$LOCAL_DEPLOY"
scp "$EXECUTABLE" worker1:~/vector_dot_mpi
scp "$EXECUTABLE" worker2:~/vector_dot_mpi

echo "MPI executable deployed to Master, Worker1 and Worker2."
