#!/usr/bin/env bash

# Get project root
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

echo "Clearing Game Boy Emulator savestates..."
if [ -d "$PROJECT_ROOT/savestates" ]; then
    rm -rf "$PROJECT_ROOT"/savestates/*.bin
    echo "Savestates cleared successfully."
else
    echo "No savestates directory found."
fi
