#!/usr/bin/env bash

set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DIST_DIR="$PROJECT_ROOT/dist"
DIST_HTML="$DIST_DIR/index.html"

if [[ ! -f "$DIST_HTML" ]]; then
    echo "Missing file: $DIST_HTML"
    echo "Place opcode table output in dist/index.html"
    exit 1
fi

open_locally() {
    local target="$1"

    if command -v xdg-open >/dev/null 2>&1; then
        xdg-open "$target" >/dev/null 2>&1 &
    else
        echo "xdg-open not found. Install it once, then rerun: sudo apt install xdg-utils"
        echo "Manual fallback: $target"
        return 1
    fi
}

echo "Opening opcode reference: $DIST_HTML"
open_locally "$DIST_HTML" || true
