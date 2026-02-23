#!/usr/bin/env bash
PROJECT_ROOT="$(cd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
cd "$BUILD_DIR"
cmake ..
cmake --build . -j$(nproc)
ctest --output-on-failure
