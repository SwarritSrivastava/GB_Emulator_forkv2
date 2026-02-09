#!/usr/bin/env bash

set -e

echo "Creating issues..."

gh issue create --title "Document initial CMake Hello World setup" --label "setup,documentation,good-first-issue" --body "
## Description
Document existing CMake + Hello World setup.

## Tasks
- [ ] Review CMakeLists.txt
- [ ] Verify build
- [ ] Update README

## Acceptance Criteria
New contributor builds in <5 min.
"

gh issue create --title "Create standard project folder layout" --label "setup,good-first-issue" --body "
## Description
Organize source folders.

## Tasks
- [ ] src/
- [ ] include/
- [ ] tests/
- [ ] docs/
- [ ] Update CMake

## Acceptance Criteria
Build still works.
"

gh issue create --title "Add .gitignore for build and IDE files" --label "setup,good-first-issue" --body "
## Description
Ignore generated files.

## Tasks
- [ ] build/
- [ ] .vscode/
- [ ] *.o

## Acceptance Criteria
No junk in git.
"


gh issue create --title "Add ROM file command line support" --label "core,good-first-issue" --body "
## Description
Load ROM via argv.

## Tasks
- [ ] Read argv[1]
- [ ] Validate path
- [ ] Error handling

## Acceptance Criteria
./emu game.gb works.
"

gh issue create --title "Load ROM into memory buffer" --label "memory" --body "
## Description
Read ROM into vector.

## Tasks
- [ ] Binary open
- [ ] Read bytes
- [ ] Store buffer

## Acceptance Criteria
Size printed.
"

gh issue create --title "Parse ROM header info" --label "memory" --body "
## Description
Read cartridge metadata.

## Tasks
- [ ] Title
- [ ] Type
- [ ] Size

## Acceptance Criteria
Correct info printed.
"

gh issue create --title "Create CPU class skeleton" --label "cpu,good-first-issue" --body "
## Description
Add CPU base files.

## Tasks
- [ ] cpu.h
- [ ] cpu.cpp
- [ ] reset()

## Acceptance Criteria
Build OK.
"

gh issue create --title "Implement CPU registers" --label "cpu" --body "
## Description
Add registers.

## Tasks
- [ ] A,B,C,D,E,H,L,F
- [ ] Init
- [ ] Print

## Acceptance Criteria
Registers readable.
"

gh issue create --title "Add PC and SP registers" --label "cpu" --body "
## Description
Add PC/SP.

## Tasks
- [ ] uint16_t PC
- [ ] uint16_t SP
- [ ] Reset values

## Acceptance Criteria
Correct init.
"

gh issue create --title "Add CPU step() function" --label "cpu" --body "
## Description
Execute one opcode.

## Tasks
- [ ] Fetch opcode
- [ ] Increment PC
- [ ] Return cycles

## Acceptance Criteria
Opcode printed.
"

gh issue create --title "Implement NOP instruction (0x00)" --label "cpu,good-first-issue" --body "
## Description
Add NOP opcode.

## Tasks
- [ ] Switch case
- [ ] No-op

## Acceptance Criteria
No crash.
"

gh issue create --title "Implement HALT instruction (0x76)" --label "cpu" --body "
## Description
Add HALT.

## Tasks
- [ ] Halt flag
- [ ] Stop loop

## Acceptance Criteria
Emulator pauses.
"

gh issue create --title "Create MMU class skeleton" --label "memory" --body "
## Description
Add MMU base.

## Tasks
- [ ] mmu.h
- [ ] mmu.cpp
- [ ] read/write

## Acceptance Criteria
Build OK.
"

gh issue create --title "Map ROM to 0000-7FFF" --label "memory" --body "
## Description
Connect ROM to MMU.

## Tasks
- [ ] Read mapping
- [ ] Bounds check

## Acceptance Criteria
CPU fetch works.
"

gh issue create --title "Add Work RAM (C000-DFFF)" --label "memory" --body "
## Description
Add WRAM.

## Tasks
- [ ] 8KB buffer
- [ ] Read/write

## Acceptance Criteria
Values persist.
"

gh issue create --title "Create PPU class skeleton" --label "ppu" --body "
## Description
Add PPU base.

## Tasks
- [ ] ppu.h
- [ ] ppu.cpp
- [ ] tick()

## Acceptance Criteria
No crash.
"

gh issue create --title "Create 160x144 framebuffer" --label "ppu" --body "
## Description
Add screen buffer.

## Tasks
- [ ] 2D array
- [ ] Clear func

## Acceptance Criteria
Initialized.
"

gh issue create --title "Render solid test screen" --label "ppu" --body "
## Description
Show test color.

## Tasks
- [ ] Fill buffer
- [ ] Display window

## Acceptance Criteria
Color visible.
"

gh issue create --title "Add logging system" --label "debug,good-first-issue" --body "
## Description
Add logging macros.

## Tasks
- [ ] LOG_INFO
- [ ] LOG_ERROR
- [ ] Toggle flag

## Acceptance Criteria
Logs visible.
"

gh issue create --title "Add instruction trace mode" --label "debug" --body "
## Description
Print opcode trace.

## Tasks
- [ ] Print PC
- [ ] Print opcode

## Acceptance Criteria
Readable trace.
"

gh issue create --title "Add first test ROM runner" --label "testing" --body "
## Description
Run Blargg tests.

## Tasks
- [ ] Add ROMs
- [ ] Run loop
- [ ] Capture output

## Acceptance Criteria
Tests run.
"

gh issue create --title "Add Contribution Guidelines" --label "meta" --body "
## Description
Create CONTRIBUTING.md.

## Tasks
- [ ] Branch naming
- [ ] Review rules
- [ ] Style guide

## Acceptance Criteria
Should be clear and concise.
"

gh issue create --title "Setup PR Template" --label "meta" --body "
## Description
Standardize PRs.

## Tasks
- [ ] Add .github/pull_request_template.md

## Acceptance Criteria
Should guide contributors to provide necessary info.
"

echo "All issues created successfully!"
