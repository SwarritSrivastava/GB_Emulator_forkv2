#!/usr/bin/env bash

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
EMU="$PROJECT_ROOT/build/bin/gb_emu"
ROM="$PROJECT_ROOT/roms/Tetris 2 (USA, Europe) (SGB Enhanced).gb"

# support

info() {
    echo "[INFO] $1"
}

error() {
    echo "[ERROR] $1" >&2
    exit 1
}

require_root() {
    if [[ $EUID -ne 0 ]]; then
        error "Please run as root (use sudo)"
    fi
}

# =============================
# Detect Distro
# =============================

detect_distro() {
    if [[ -f /etc/os-release ]]; then
        . /etc/os-release
        echo "$ID"
    else
        error "Cannot detect Linux distribution"
    fi
}

# install

install_arch() {
    info "Detected Arch Linux"

    if ! command -v valgrind >/dev/null 2>&1; then
        pacman -S --noconfirm valgrind glibc-debug elfutils
    fi
}

install_fedora() {
    info "Detected Fedora/RHEL"

    if ! command -v valgrind >/dev/null 2>&1; then
        dnf install -y valgrind glibc-debuginfo glibc-debuginfo-common elfutils
    fi
}

install_debian() {
    info "Detected Debian/Ubuntu"

    if ! command -v valgrind >/dev/null 2>&1; then
        apt update
        apt install -y valgrind libc6-dbg elfutils
    fi
}

main() {

    require_root

    DISTRO=$(detect_distro)

    info "Detected distro: $DISTRO"

    case "$DISTRO" in
        arch|cachyos)
            install_arch
            ;;
        fedora|rhel|centos)
            install_fedora
            ;;
        debian|ubuntu)
            install_debian
            ;;
        *)
            error "Unsupported distro: $DISTRO"
            ;;
    esac

    info "Valgrind installation complete"

    if [[ ! -x "$EMU" ]]; then
        error "Emulator not found: $EMU"
    fi

    if [[ ! -f "$ROM" ]]; then
        error "ROM not found: $ROM"
    fi

    info "Running Valgrind..."

    valgrind \
        --leak-check=full \
        --show-leak-kinds=all \
        --track-origins=yes \
        "$EMU" "$ROM"

    info "Done."
}

main