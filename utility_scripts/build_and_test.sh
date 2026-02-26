#!/usr/bin/env bash

set -e # Exit on error

# Get project root
PROJECT_ROOT="$(cd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/.." && pwd)"

# Build
BUILD_DIR="$PROJECT_ROOT/build"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
cmake ..
cmake --build . -j$(nproc)

# Test
ctest --output-on-failure
