# GB_Emulator
**Game Boy (DMG-01) Emulator**

Early-stage emulator for the original Game Boy (Dot-Matrix Game – DMG-01).

Currently focused on **accurate CPU core** and **basic MMU / cartridge handling**.

Repository CI now includes a structured Discord webhook notification flow for push summaries and test status updates.

![Game Boy DMG-01](https://placehold.co/480x432/EEE/31343C?text=Game+Boy+DMG-01)  
*(classic gray brick – the real hardware this project aims to emulate)*

## Current Status (March 2026)

**What works reasonably well:**

- Z80-like CPU core (Sharp LR35902)
    - Most 8-bit & 16-bit instructions implemented (via `opcodes.cpp` + `cb_opcodes.cpp`)
    - CB-prefixed opcodes table
    - Correct reset state (AF=01B0h, BC=0013h, DE=00D8h, HL=014Dh, SP=FFFEh, PC=0100h)
    - Basic cycle-accurate stepping (PC increments, NOPs, HALT behavior)
    - Unknown opcode handling (graceful fallback)

- MMU / Memory Map
    - ROM loading & mapping (fixed bank 0 + switchable banks not yet)
    - Cartridge header parsing (title, MBC type, ROM/RAM size, checksum)
    - WRAM (C000–DFFF), basic echo RAM behavior
    - Read-only ROM protection
    - Zero-initialized memory on startup

- Very basic execution loop
    - Loads & runs ROM until stopped (currently limited instructions executed)
    - Prints CPU state after reset

**What is NOT implemented yet:**

- PPU / rendering (no screen output)
- Timer, DMA, interrupts (IME, vblank, joypad, etc.)
- Sound
- Input (joypad)
- MBC1/MBC3/... bank switching (only basic ROM mapping)
- Proper frame timing & synchronization
- Save RAM (external cartridge RAM)
- Display window / SDL / graphics backend

**Tested ROMs (so far):**

- Pokémon Red (executes ~2400 instructions after reset)
- Tetris 2 (executes ~140 instructions after reset)

Both ROMs pass header check & basic mapping, but obviously stop very early (no graphics/input).

## Project Structure

```text
GB_Emulator/
├── include/              → public headers
│   ├── cartridge.hpp
│   ├── mmu.hpp
│   ├── ProcessingUnit.hpp   ← main CPU class
│   └── ...
├── src/
│   ├── core/
│   │   ├── cpu/          ← CPU emulation heart
│   │   └── memory/       ← MMU
│   ├── display/          ← future rendering stub (port.cpp)
│   └── main.cpp
├── tests/                → unit tests (GoogleTest)
│   ├── mmu_tests.cpp
│   └── ProcessingUnitTest.cpp
├── roms/                 → test ROMs
├── utility_scripts/      → quality-of-life scripts
│   ├── build_and_test.sh
│   ├── build_and_run.sh
│   ├── run_game.sh
│   └── valgrind_tests.sh   ← memory leak checking
├── documentation/        → Doxygen config
├── CMakeLists.txt
└── vcpkg.json            → dependency management
```

## Build & Run

```bash
Bash# Build + run unit tests
./utility_scripts/build_and_test.sh

# Build + run Pokémon Red (or edit script for other ROM)
./utility_scripts/build_and_run.sh

# Just run a ROM (after building)
./utility_scripts/run_game.sh

# Run valgrind leak check (requires sudo on some systems)
sudo ./utility_scripts/valgrind_tests.sh
```
Typical output when launching a game:
```txt
ROM Size: 1024K
Title:  POKEMON RED
Type:  0x13    ROM: 0x5    RAM: 0x3
Header OK

ROM successfully mapped to MMU memory ...

Initial State (Post-Reset):
AF: 01B0    BC: 0013    DE: 00D8    HL: 014D
SP: FFFE    PC: 0100
Flags: Z - H C    [IME:0  HALT:0]

Success!
2399 instructions executed
```
All 14 unit tests currently pass:

CPU reset values, HALT, PC stepping, unknown opcodes, MMU zero-init, write protection, ROM mapping ...

## Testing
 > Unit tests → GoogleTest (CPU reset state, instruction stepping, MMU read/write/aliasing, ROM mapping edge cases)
 
 > 100% pass rate on current suite (~14 tests)
 
 > Valgrind → no leaks detected on short runs

 > Doxygen → documentation generation (run doxygen DoxygenConfig in documentation/)
