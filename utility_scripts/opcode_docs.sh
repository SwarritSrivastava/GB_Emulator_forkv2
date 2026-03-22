#!/usr/bin/env bash

set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OPCODE_DIR="$PROJECT_ROOT/int/generate-gb-opcodes"
DIST_DIR="$OPCODE_DIR/dist"
DIST_HTML="$DIST_DIR/index.html"
DIST_JS="$DIST_DIR/main.js"

if [[ ! -d "$OPCODE_DIR" ]]; then
    echo "Missing directory: $OPCODE_DIR"
    echo "Expected opcode table project under int/generate-gb-opcodes"
    exit 1
fi

ensure_dependencies() {
    if [[ ! -d "$OPCODE_DIR/node_modules" ]]; then
        echo "Installing opcode table dependencies..."
        npm --prefix "$OPCODE_DIR" ci --no-audit --no-fund
    fi
}

build_opcode_docs() {
    if [[ -f "$DIST_HTML" && -f "$DIST_JS" ]]; then
        return
    fi

    echo "Building local opcode docs..."
    (
        cd "$OPCODE_DIR"
        NODE_OPTIONS="--openssl-legacy-provider" npm run build
    )
}

open_locally() {
    local target="$1"

    if command -v xdg-open >/dev/null 2>&1; then
        xdg-open "$target" >/dev/null 2>&1 &
    elif command -v open >/dev/null 2>&1; then
        open "$target"
    elif command -v start >/dev/null 2>&1; then
        start "$target"
    else
        echo "No opener found. Open this file manually: $target"
        return 1
    fi
}

ensure_dependencies
build_opcode_docs

echo "Opening opcode reference: $DIST_HTML"
open_locally "$DIST_HTML" || true
