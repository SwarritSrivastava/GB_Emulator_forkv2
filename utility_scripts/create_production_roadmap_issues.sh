#!/usr/bin/env bash

set -euo pipefail

# Creates one parent roadmap issue + detailed child issues.
#
# Default behavior:
# - Detects repo from git remote origin.
# - Creates child issues first.
# - Creates a parent issue with a task-list linking child issue numbers.
#
# Usage:
#   ./utility_scripts/create_production_roadmap_issues.sh
#   ./utility_scripts/create_production_roadmap_issues.sh --repo owner/name
#   ./utility_scripts/create_production_roadmap_issues.sh --dry-run

REPO=""
DRY_RUN=0

while [[ $# -gt 0 ]]; do
    case "$1" in
        --repo)
            REPO="${2:-}"
            shift 2
            ;;
        --dry-run)
            DRY_RUN=1
            shift
            ;;
        -h|--help)
            cat <<'EOF'
Usage: create_production_roadmap_issues.sh [--repo owner/name] [--dry-run]

Options:
  --repo      Target GitHub repository (owner/name).
  --dry-run   Print what would be created without calling GitHub.
EOF
            exit 0
            ;;
        *)
            echo "Unknown argument: $1" >&2
            exit 1
            ;;
    esac
done

if [[ -z "$REPO" ]]; then
    if ! remote_url=$(git config --get remote.origin.url 2>/dev/null); then
        echo "Could not detect origin remote. Use --repo owner/name." >&2
        exit 1
    fi

    case "$remote_url" in
        git@github.com:*.git)
            REPO="${remote_url#git@github.com:}"
            REPO="${REPO%.git}"
            ;;
        https://github.com/*.git)
            REPO="${remote_url#https://github.com/}"
            REPO="${REPO%.git}"
            ;;
        https://github.com/*)
            REPO="${remote_url#https://github.com/}"
            ;;
        *)
            echo "Unsupported origin URL format: $remote_url" >&2
            echo "Use --repo owner/name." >&2
            exit 1
            ;;
    esac
fi

if [[ "$DRY_RUN" -eq 0 ]]; then
    if ! command -v gh >/dev/null 2>&1; then
        echo "GitHub CLI (gh) is required." >&2
        exit 1
    fi

    if ! gh auth status >/dev/null 2>&1; then
        echo "gh is not authenticated. Run: gh auth login" >&2
        exit 1
    fi
fi

declare -a CHILD_TITLES=()
declare -a CHILD_NUMBERS=()
declare -a CHILD_URLS=()

create_child_issue() {
    local title="$1"
    local body="$2"

    CHILD_TITLES+=("$title")

    if [[ "$DRY_RUN" -eq 1 ]]; then
        CHILD_NUMBERS+=("TBD")
        CHILD_URLS+=("N/A")
        printf '[dry-run] child: %s\n' "$title"
        return
    fi

    local url
    url=$(gh issue create --repo "$REPO" --title "$title" --body "$body")
    local number="${url##*/}"

    CHILD_NUMBERS+=("$number")
    CHILD_URLS+=("$url")

    printf 'created child #%s: %s\n' "$number" "$title"
}

create_parent_issue() {
    local title="$1"
    local body="$2"

    if [[ "$DRY_RUN" -eq 1 ]]; then
        printf '[dry-run] parent: %s\n' "$title"
        return
    fi

    local url
    url=$(gh issue create --repo "$REPO" --title "$title" --body "$body")
    printf 'created parent issue: %s\n' "$url"
}

create_child_issue \
"[Roadmap][Architecture] Define production class model and execution boundaries" \
"$(cat <<'EOF'
## Why
The current system works, but coupling and ownership boundaries are still soft. We need stable class boundaries before finishing CPU + adding PPU/timers.

## Scope
- Create a production class model with clear ownership:
  - `Emulator` (or `GameBoy`) root coordinator
  - `CPU`
  - `Bus`/`MMU`
  - `PPU`
  - `Timer`
  - `InterruptController`
  - `Cartridge`
- Define single ownership and lifetime for each component.
- Define who can mutate which state.

## Deliverables
- [ ] Architecture document (component diagram + data flow + timing flow)
- [ ] Header/API contract per component
- [ ] Constructor wiring plan and dependency graph
- [ ] Migration checklist from current implementation to new class model

## Acceptance Criteria
- No cyclic dependencies between core components
- Clear boundary for memory access (CPU/PPU/Timer use bus, not direct arrays)
- Team can implement against the contract without ambiguity
EOF
)"

create_child_issue \
"[Roadmap][CPU] Enforce opcode fetch/PC contract and remove ambiguity" \
"$(cat <<'EOF'
## Why
Incorrect or inconsistent PC handling can silently break opcode correctness and timing.

## Scope
- Freeze one fetch contract:
  - `CPU::step()` fetches opcode at `PC`, increments PC once.
  - Handlers only fetch operands according to instruction format.
- Remove inconsistent pre-increments in immediate/jump handlers.
- Introduce helper methods (`fetch8`, `fetch16`, signed offset fetch).

## Deliverables
- [ ] Documented CPU fetch contract in code + CONTRIBUTING
- [ ] Refactor opcode handlers to contract-compliant operand fetch
- [ ] Add regression tests for PC movement (before/after instruction)
- [ ] Ensure CB-prefix path also follows contract

## Acceptance Criteria
- Every opcode test that touches immediate data validates expected PC delta
- No handler performs unexplained extra PC increments
- Prefix and non-prefix execution produce deterministic PC behavior
EOF
)"

create_child_issue \
"[Roadmap][CPU] Complete remaining base opcodes (C1-FF) and illegal-op behavior" \
"$(cat <<'EOF'
## Why
Runtime `DUMMY(...)` placeholders block production readiness and can hide major correctness gaps.

## Scope
- Implement all remaining base opcodes from `0xC1` to `0xFF`.
- Replace placeholder behavior with real implementation or strict illegal opcode handling.
- Ensure stack, call/ret, interrupt toggles, and immediate operations are correct.

## Deliverables
- [ ] All missing opcode handlers implemented
- [ ] `DUMMY(...)` removed from production opcode paths
- [ ] Per-opcode tests including cycle counts + flag behavior + memory side effects
- [ ] Illegal opcode policy documented and tested

## Acceptance Criteria
- `opcode_progress.py` reports base opcode completion 100%
- Full opcode suite passes with no placeholder execution in runtime path
- Stack/flow-control opcodes validated through targeted integration tests
EOF
)"

create_child_issue \
"[Roadmap][Bus/MMU] Refactor memory map into production bus architecture" \
"$(cat <<'EOF'
## Why
PPU/timers/interrupts require memory-mapped IO and cycle-aware behavior. Flat/partial mapping is not enough.

## Scope
- Introduce memory map regions with explicit routing:
  - ROM, VRAM, WRAM, OAM, HRAM, IO registers, IE
- Implement read/write behavior by region and mode restrictions where applicable.
- Add DMA and mirror behavior planning hooks.

## Deliverables
- [ ] Bus/MMU interface with region-routed read/write
- [ ] IO register stubs with documented behavior
- [ ] Tests for mapping boundaries and write-protection rules
- [ ] Migration plan from old flat memory usage

## Acceptance Criteria
- Address map behavior is deterministic and test-covered
- CPU/PPU/Timer access bus through one canonical interface
- No accidental direct writes bypassing mapping rules
EOF
)"

create_child_issue \
"[Roadmap][Timing] Add interrupt controller + timer subsystem with cycle stepping" \
"$(cat <<'EOF'
## Why
Accurate interrupts and timers are mandatory for game boot reliability and stable emulation timing.

## Scope
- Implement IF/IE handling and interrupt arbitration priority.
- Implement IME behavior (`EI` delayed enable, `DI`, `RETI`).
- Implement timer registers/divider progression (DIV/TIMA/TMA/TAC) per cycles.

## Deliverables
- [ ] Interrupt controller class and API
- [ ] Timer class stepped by CPU cycles
- [ ] Unit tests for interrupt servicing and timer overflow/reload
- [ ] Integration tests for halt/interrupt wake-up behavior

## Acceptance Criteria
- Interrupt entry/return stack behavior validated
- Timer overflow triggers interrupt correctly
- HALT behavior with pending/active interrupts matches expected hardware semantics
EOF
)"

create_child_issue \
"[Roadmap][PPU Phase 1] Implement mode state machine and scanline timing" \
"$(cat <<'EOF'
## Why
PPU timing must exist before reliable rendering and many test ROMs depend on LY/STAT correctness.

## Scope
- Implement PPU modes and transitions:
  - OAM scan, drawing, HBlank, VBlank
- Track dot/cycle progression per scanline and frame.
- Implement LY/STAT/LCDC register core behavior needed for timing.

## Deliverables
- [ ] `PPU::step(cycles)` mode machine
- [ ] LY increment/reset logic
- [ ] STAT mode flags + interrupt trigger points
- [ ] Tests for line transitions and VBlank boundaries

## Acceptance Criteria
- Frame timing is stable over long runs
- LY/STAT transitions pass deterministic tests
- VBlank interrupt timing occurs at expected boundaries
EOF
)"

create_child_issue \
"[Roadmap][PPU Phase 2] Implement renderer pipeline (BG/Window/Sprites)" \
"$(cat <<'EOF'
## Why
After timing correctness, visual output and compatibility require full DMG pixel pipeline behavior.

## Scope
- Render background layer from tile maps/data.
- Render window layer with proper enable/window position rules.
- Render sprites with priority, palette, flipping, and per-line limits.
- Produce framebuffer output (160x144) from PPU output state.

## Deliverables
- [ ] Background rendering implementation
- [ ] Window rendering implementation
- [ ] Sprite rendering implementation
- [ ] Framebuffer integration tests for representative cases

## Acceptance Criteria
- Stable image output on known-good ROM scenes
- Layer priority behavior matches expected DMG ordering
- Rendering features are testable and not hardcoded to one game
EOF
)"

create_child_issue \
"[Roadmap][Testing] Build correctness harness from unit to ROM-level regression" \
"$(cat <<'EOF'
## Why
Current unit coverage is good, but production quality requires stronger integration and hardware-behavior validation.

## Scope
- Keep unit tests for opcodes/flags/cycles.
- Add step-driven integration tests using real fetch/decode path.
- Add automated ROM-level regression suite (CPU and timing-focused ROMs).
- Add golden-state snapshots for critical boot phases.

## Deliverables
- [ ] Integration tests that execute via `cpu.step(mmu)` path
- [ ] Regression ROM harness with pass/fail extraction
- [ ] Coverage report focused on critical CPU/PPU/timer paths
- [ ] Test documentation for contributors

## Acceptance Criteria
- Regressions in PC/flags/cycles are caught automatically
- At least one ROM-level suite is runnable in CI
- New opcode/timing changes cannot merge without test validation
EOF
)"

create_child_issue \
"[Roadmap][Performance] Instrument, benchmark, and optimize for production speed" \
"$(cat <<'EOF'
## Why
We want production-grade speed without sacrificing correctness. Optimization must be measured, not guessed.

## Scope
- Define performance KPIs and benchmark scenarios.
- Add lightweight profiling hooks (cycles/sec, frame time, hotspots).
- Optimize hot paths (dispatch, memory reads/writes, render loops) after correctness baseline.
- Add release build profile configuration and benchmark script.

## Deliverables
- [ ] Benchmark script and repeatable benchmark dataset
- [ ] Baseline metrics committed in docs
- [ ] Optimization changelog with before/after numbers
- [ ] Performance CI check (non-blocking at first, then enforce)

## Acceptance Criteria
- Emulator sustains target real-time factor in release mode on defined baseline hardware
- No optimization merged without measured delta
- Correctness tests still pass after each optimization pass
EOF
)"

create_child_issue \
"[Roadmap][CI/Quality] Enforce production gates, coding standards, and contributor workflow" \
"$(cat <<'EOF'
## Why
To hit and maintain an 8+ quality bar, the repo needs enforceable quality gates.

## Scope
- Align workflow triggers with intended branch policy.
- Add format/lint/static checks and fail-fast policies.
- Add quality gates for missing placeholders, test failures, and benchmark regressions.
- Standardize commit/PR templates and review checklist.

## Deliverables
- [ ] CI trigger policy documented and enforced
- [ ] Formatting/linting/static analysis in workflow
- [ ] Placeholder guard (fail if `DUMMY(...)` present in production paths)
- [ ] PR checklist for correctness + performance + docs

## Acceptance Criteria
- CI catches style, correctness, and regression issues before merge
- Branch protections reflect required checks
- Workflow/docs are synchronized and current
EOF
)"

TASK_LIST=""
for i in "${!CHILD_TITLES[@]}"; do
    title="${CHILD_TITLES[$i]}"
    num="${CHILD_NUMBERS[$i]}"

    if [[ "$num" == "TBD" ]]; then
        TASK_LIST+="- [ ] ${title} (issue link added when created)"$'\n'
    else
        TASK_LIST+="- [ ] #${num} - ${title}"$'\n'
    fi
done

PARENT_BODY="$(cat <<EOF
## Mission
Upgrade the emulator to an **8+/10 production-quality baseline** with strong correctness, maintainability, and measurable performance.

## Target Outcomes
- Full opcode coverage and deterministic CPU behavior
- Production-grade class architecture and bus model
- Functional PPU with correct timing and rendering pipeline
- Robust regression system from unit tests to ROM-level validation
- Measured performance improvements with reproducible benchmarks

## Performance Goal
- Define and track a baseline real-time factor on target hardware
- Optimize for high throughput while preserving correctness
- Keep optimization changes benchmark-backed and regression-safe

## Master Deliverables
- [ ] Architecture contract finalized and implemented
- [ ] CPU fetch contract + remaining opcodes completed
- [ ] Bus/MMU, interrupts, timers, and PPU integrated via cycle stepping
- [ ] Regression harness + CI quality gates
- [ ] Performance benchmark suite with documented targets/results

## Sub-Issues (Execution Plan)
${TASK_LIST}

## Definition of Done
- CPU base + CB opcode behavior is complete and test-verified
- PPU/timer/interrupt core works with stable frame/timing progression
- CI enforces quality gates for correctness and code health
- Benchmarks show sustained production performance and no major regressions
EOF
)"

create_parent_issue \
"[Roadmap] Production-grade GB emulator plan (8+ quality, correctness, and performance)" \
"$PARENT_BODY"

if [[ "$DRY_RUN" -eq 1 ]]; then
    echo
    echo "dry-run complete: no GitHub issues were created."
else
    echo
    echo "done: roadmap issues created in ${REPO}."
fi
