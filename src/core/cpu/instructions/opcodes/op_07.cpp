#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_ld_hl_b) // 0x70
DUMMY(op_ld_hl_c) // 0x71
DUMMY(op_ld_hl_d) // 0x72
DUMMY(op_ld_hl_e) // 0x73
DUMMY(op_ld_hl_h) // 0x74
DUMMY(op_ld_hl_l) // 0x75

int op_halt(ProcessingUnit &cpu, MMU &mmu) // 0x76
{
    cpu.setHalt(true);
    return totalMachineCycles(1);
}

DUMMY(op_ld_hl_a) // 0x77
DUMMY(op_ld_a_b) // 0x78
DUMMY(op_ld_a_c_79) // 0x79
DUMMY(op_ld_a_d) // 0x7A
DUMMY(op_ld_a_e) // 0x7B
DUMMY(op_ld_a_h) // 0x7C
DUMMY(op_ld_a_l) // 0x7D
DUMMY(op_ld_a_hl) // 0x7E
DUMMY(op_ld_a_a) // 0x7F