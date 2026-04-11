# Contributing to GB_Emulator

Thanks for contributing.

This project is a Game Boy (DMG-01) emulator focused on CPU/MMU correctness with incremental opcode bring-up and test-first changes.

This guide defines:
- code and review standards
- repository architecture and object model
- a repeatable opcode implementation workflow
- a repeatable unit test workflow
- known technical debt and practices we should phase out

## Table of Contents

1. Scope and goals
2. Project architecture
3. Build and test workflow
4. Coding standards
5. Opcode implementation standard
6. Opcode unit test standard
7. Pull request checklist
8. Known bad practices and debt
9. Templates

## 1) Scope and goals

Current priority:
- Implement LR35902 base + CB opcodes correctly
- Keep instruction dispatch deterministic and readable
- Maintain strict unit coverage for every implemented opcode
- Catch regressions through local tests and CI

When contributing, prefer correctness and consistency over cleverness.

## 2) Project architecture

High-level layout:

```text
include/
  ProcessingUnit.hpp      # CPU state + register/flag API
  mmu.hpp                 # memory bus API
  opcodes.hpp             # base opcode declarations
  cb_opcodes.hpp          # CB-prefixed opcode declarations
  opcode_table.hpp        # instruction table types + externs

src/core/cpu/
  ProcessingUnit.cpp      # CPU step(), reset(), register access
  instructions/
    opcode_table.cpp      # 256-entry base + 256-entry CB dispatch tables
    opcodes/op_00.cpp ... op_0F.cpp
    cb_opcodes/cb_op_00.cpp ... cb_op_0F.cpp

src/core/memory/
  mmu.cpp                 # read/write + ROM mapping

tests/
  ProcessingUnitTest.cpp
  mmu_tests.cpp
  op_tests/*.cpp          # opcode-specific tests
```

### Object model and class responsibilities

#### `ProcessingUnit`
- Owns CPU registers (`A,F,B,C,D,E,H,L,SP,PC`) and execution state (`IME`, `halt`, `stop`)
- Exposes register and flag helpers (`reg()`, `get_flag_*()`, `setFlag()`)
- Executes one instruction in `step(MMU&)` by:
  1. fetching opcode from `MMU` at `PC`
  2. incrementing `PC`
  3. dispatching to `instructionTable[opcode]`

#### `MMU`
- Owns memory arrays and bus behavior
- Handles ROM mapping (`map_rom`) and guarded writes
- Provides `read(u16)` and `write(u16, u8)` used by opcodes

#### Dispatch tables (`opcode_table.cpp`)
- `instructionTable[256]`: base opcode function pointers
- `cbInstructionTable[256]`: CB-prefixed opcode function pointers
- Order in tables must exactly match opcode value

## 3) Build and test workflow

### Recommended local flow

```bash
./utility_scripts/build_and_test.sh
```

### Focused test run while developing opcodes

```bash
cd build
ctest -R "(OPCODE_NAME_OR_PREFIX)" --output-on-failure
```

Examples:
- `ctest -R "(SUB_|SBC_)" --output-on-failure`
- `ctest -R "(AND_|XOR_)" --output-on-failure`

### CI notes
- Test workflow: `.github/workflows/test-init-response-protocol.yml`
- Pages workflow: `.github/workflows/static.yml`

## 4) Coding standards

### C++ conventions (project standard)
- Language standard: C++23
- Keep opcode functions small and explicit
- Prefer fixed-width aliases (`u8`, `u16`, etc.) from `common.hpp`
- No hidden side effects in opcode implementations
- Keep naming aligned with `opcodes.hpp`/`cb_opcodes.hpp`

### Opcode implementation rules
- Always return machine cycles via:
  - `constexpr int machine_cycles = 4;`
  - `#define totalMachineCycles(n) ((n) * machine_cycles)`
- Register-only opcodes: usually `totalMachineCycles(1)`
- `(HL)` memory opcodes: usually `totalMachineCycles(2)`
- Immediate fetch opcodes must use `cpu.inc_pc()`/`mmu.read()` consistently
- Update only flags defined by LR35902 behavior for that instruction
- For compare operations (`CP`): do not modify register `A`

### Testing conventions
- Use `TEST_F(OpcodesCPUTest, <DescriptiveName>)`
- Validate all of the following unless not relevant:
  - cycle count
  - result register/memory value
  - all affected flags
  - non-target state remains unchanged where meaningful

## 5) Opcode implementation standard

Use this sequence for any new opcode or opcode update.

1. Confirm exact behavior in:
   - `utility_scripts/opcode_lookup.py` (mnemonic, cycles, flags, description)
   - trusted LR35902 references if behavior is ambiguous
2. Add or update declaration in:
   - `include/opcodes.hpp` or `include/cb_opcodes.hpp`
3. Implement in the correct source bucket:
   - base: `src/core/cpu/instructions/opcodes/op_XX.cpp`
   - CB: `src/core/cpu/instructions/cb_opcodes/cb_op_XX.cpp`
4. Wire function pointer in:
   - `src/core/cpu/instructions/opcode_table.cpp`
5. Add or update tests in mirrored location:
   - `tests/op_tests/op_XX_tests.cpp` or `tests/op_tests/cb_op_XX_tests.cpp`
6. Build and run focused tests
7. Run full test suite before merge

### Flag correctness checklist

Before finalizing an opcode, verify:
- `Z` set when result is zero (or forced clear/set for instructions that define it)
- `N` matches add/sub semantics
- `H` uses nibble carry/borrow logic
- `C` uses byte carry/borrow logic

## 6) Opcode unit test standard

Minimum coverage per opcode:
- one normal case
- one edge case (zero result, half-carry/half-borrow, carry/borrow, sign wrap)
- for `(HL)` variants: validate memory read/write path and cycle count

For arithmetic/logical families, include at least one test that validates each relevant flag transition.

## 7) Pull request checklist

Before opening PR:
- [ ] Compiles cleanly
- [ ] New/updated opcode wired in dispatch table
- [ ] Tests added/updated for every changed opcode
- [ ] `ctest --output-on-failure` passes locally
- [ ] No placeholder `DUMMY(...)` left for implemented opcodes
- [ ] Function names and comments match opcode numbers

In PR description, include:
- opcode range changed
- behavior summary
- tests added
- any known limitations

## 8) Known bad practices and debt

These exist today. Do not add more of them. Prefer improving them incrementally.

1. `DUMMY(...)` opcode placeholders in production paths
   - Risk: silent false positives and incorrect emulation behavior
   - Standard: replace dummies with real implementations + tests before considering an opcode done

2. Relative include depth (`../../../../../include/...`)
   - Risk: fragile include paths and harder refactors
   - Standard: migrate toward project-root includes via CMake include dirs

3. Inconsistent naming for a few opcode symbols (example suffixes like `_79`, `_4f`)
   - Risk: harder lookup and maintenance
   - Standard: preserve existing names for compatibility, but avoid introducing new irregular names

4. Macro-heavy utilities in `common.hpp` (`BIT`, `BIT_SET`, `BETWEEN`) and global `typedef` aliases
   - Risk: macro side effects and weaker type hygiene
   - Standard: prefer `constexpr` helpers and `using` aliases in new code

5. Missing formatter/linter config in repo root
   - Risk: inconsistent style
   - Standard: if a formatter is introduced later, apply project-wide once and enforce in CI

6. `valgrind_tests.sh` requiring root and installing packages at runtime
   - Risk: non-reproducible local behavior and elevated privilege requirement
   - Standard: keep as CI utility for now; prefer pre-provisioned toolchains when possible

7. Multiple files define the same `OpcodesCPUTest` fixture name independently
   - Note: valid because each test file is a separate translation unit
   - Standard: allowed, but consider fixture extraction if setup becomes complex

## 9) Templates

### A) Base opcode implementation template (register source)

```cpp
int op_example(ProcessingUnit& cpu, MMU& mmu) // 0xXX
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 v = cpu.reg(ProcessingUnit::Register::B);
    const u8 result = static_cast<u8>(a /* op */ v);

    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}
```

### B) Base opcode implementation template (`(HL)` source)

```cpp
int op_example_hl(ProcessingUnit& cpu, MMU& mmu) // 0xXX
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 v = mmu.read(cpu.get_hl());
    const u8 result = static_cast<u8>(a /* op */ v);

    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(2);
}
```

### C) Compare (`CP`) template

```cpp
int op_cp_x(ProcessingUnit& cpu, MMU& mmu) // 0xBX
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 v = cpu.reg(ProcessingUnit::Register::B);
    const int result = a - v;
    const int half = (a & 0x0F) - (v & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    return totalMachineCycles(1);
}
```

### D) Opcode test template

```cpp
TEST_F(OpcodesCPUTest, OPCODE_Name)
{
    cpu.reg(ProcessingUnit::Register::A) = 0xF0;
    cpu.reg(ProcessingUnit::Register::B) = 0x0F;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_example(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xFF);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}
```

---

If you are unsure about an opcode behavior, do not guess. Add a short note in your PR and link the reference used.
