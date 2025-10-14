#!/usr/bin/env bash
# Moves build artifacts into src/py/bin/
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd -P)"
PYDIR="$ROOT"
BINDIR="$PYDIR/bin"

mkdir -p "$BINDIR"

mv -v "$PYDIR"/chess_engine.exe "$BINDIR" 2>/dev/null || true
mv -v "$PYDIR"/libchess.dll "$BINDIR" 2>/dev/null || true
mv -v "$PYDIR"/libchess.a "$BINDIR" 2>/dev/null || true
mv -v "$PYDIR"/*.o "$BINDIR" 2>/dev/null || true
mv -v "$PYDIR"/engine_out.txt "$BINDIR" 2>/dev/null || true

echo "Build artifacts moved to $BINDIR"