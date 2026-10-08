#!/usr/bin/env bash
set -euo pipefail

N="${1:-}"
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
EXECUTABLE="$ROOT_DIR/build/vector_dot_sequential"

if [[ -z "$N" ]]; then
    echo "Usage: $0 <vector_size>"
    exit 1
fi

if [[ ! -x "$EXECUTABLE" ]]; then
    echo "Sequential executable not found. Run: make sequential"
    exit 1
fi

exec "$EXECUTABLE" "$N"
