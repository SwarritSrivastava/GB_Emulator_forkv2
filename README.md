# GB_Emulator

Game Boy (DMG-01) emulator project focused on building an accurate LR35902 CPU core, memory subsystem, and a reliable CI/testing pipeline.

```bash
❯ ./utility_scripts/opcode_progress.py
- Base Opcodes: [████████████████████] 100.0% (256/256)
- CB Opcodes:   [████████████████████] 100.0% (256/256)
- Combined:     [████████████████████] 100.0% (512/512)
```

## Current Focus

- CPU core progression using opcode-function tables (`instructionTable` and `cbInstructionTable`).
- MMU behavior, ROM mapping, and cartridge metadata parsing.
- Test-first iteration for opcode implementations (`tests/op_tests`).
- CI protocol named **TIRP** (Test Init Response Protocol) with automated Discord reporting.

## Current Code Structure

```text
GB_Emulator/
├── include/
│   ├── ProcessingUnit.hpp
│   ├── mmu.hpp
│   ├── cartridge.hpp
│   ├── opcode_table.hpp
│   ├── opcodes.hpp
│   └── cb_opcodes.hpp
├── src/
│   ├── core/
│   │   ├── cpu/
│   │   │   ├── ProcessingUnit.cpp
│   │   │   └── instructions/
│   │   │       ├── opcode_table.cpp
│   │   │       ├── opcodes/        # op_00.cpp ... op_0F.cpp
│   │   │       └── cb_opcodes/     # cb_op_00.cpp ... cb_op_0F.cpp
│   │   └── memory/
│   ├── io/
│   │   └── port.cpp
│   └── main.cpp
├── tests/
│   ├── ProcessingUnitTest.cpp
│   ├── mmu_tests.cpp
│   ├── sanity_tests.cpp
│   └── op_tests/                   # opcode + CB-opcode test files
├── utility_scripts/
│   ├── build_and_test.sh
│   ├── build_and_run.sh
│   ├── run_game.sh
│   ├── valgrind_tests.sh
│   ├── opcode_lookup.py
│   ├── opcode_docs.sh
│   ├── make_opcode_skeleton.sh
│   ├── make_cb_opcode_skeleton.sh
│   └── groq_push_summary.py
├── pipline/
│   └── SetUp.md                    # Discord + Cloudflare + workflow integration guide
└── .github/workflows/
    ├── test-init-response-protocol.yml
    └── static.yml
```

## Opcode Architecture

- Opcode declarations live in `include/opcodes.hpp` and `include/cb_opcodes.hpp`.
- Dispatch tables are defined in `src/core/cpu/instructions/opcode_table.cpp`.
- Base opcode implementation files are grouped under `src/core/cpu/instructions/opcodes/`.
- CB-prefixed opcode implementation files are grouped under `src/core/cpu/instructions/cb_opcodes/`.
- Matching tests are mirrored under `tests/op_tests/`.

This layout keeps opcode declarations, implementations, and tests aligned and easy to expand incrementally.

## Build and Run

```bash
# Configure + build + run all tests
./utility_scripts/build_and_test.sh

# Configure + build + run emulator with first ROM in roms/
./utility_scripts/build_and_run.sh

# Run emulator directly (expects an existing build)
./utility_scripts/run_game.sh

# Run valgrind protocol (script requires sudo/root)
sudo ./utility_scripts/valgrind_tests.sh
```

Direct binary usage:

```bash
./build/bin/gb_emu <path-to-rom.gb>
```

## Workflow and Pipeline

### 1) TIRP CI Workflow

File: `.github/workflows/test-init-response-protocol.yml`

Triggers:
- `push`
- `pull_request` (targeting `main` / `master`)
- `workflow_dispatch`
- `repository_dispatch` with type `run-test`

Core stages:
- Build + test (`build_and_test.sh`)
- Valgrind protocol (`valgrind_tests.sh`)
- AI summary generation (`groq_push_summary.py`)
- Discord payload preparation + webhook notification
- Final fail/pass gate based on test and valgrind exit codes

### 2) Pages Deploy Workflow

File: `.github/workflows/static.yml`

Triggers:
- tag pushes matching `deploy-*`
- `workflow_dispatch`
- `repository_dispatch` with type `deploy-pages`

Publishes `documentation/html` through GitHub Pages deployment actions.

## Extended Automation Setup

The full Discord bot + Cloudflare Worker + GitHub Actions operational setup is documented in:

- `pipline/SetUp.md`

That document includes:
- Role-based slash command flow (`/test`, `/deploy`, issue/PR commands)
- `repository_dispatch` wiring
- secret/token setup and security notes
- failure notification and maintainer tagging behavior

## Documentation

- Doxygen config: `DoxygenConfig`
- Generated docs: `documentation/html/`
- Pages deployment source: `documentation/html/`

## Notes

- This project is still in active bring-up; CPU and MMU areas are evolving quickly.
- Graphics/PPU, sound, interrupts/timers, and full hardware timing are still under development.
