#!/usr/bin/env bash

set -e  # Exit on error

# Get project root
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"

# Build
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake ..
cmake --build . -j"$(nproc)"

echo
echo "Running"
echo

# Get ROM files
ROM_FILES=("$PROJECT_ROOT"/roms/*)

# Check if any ROM exists
if [ ${#ROM_FILES[@]} -gt 0 ]; then
    FIRST_ROM="${ROM_FILES[0]}"
    echo "Launching: $FIRST_ROM"

    ./bin/gb_emu "$FIRST_ROM"
else
    echo "No ROM files found in $PROJECT_ROOT/roms"
fi