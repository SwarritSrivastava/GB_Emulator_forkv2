#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_ld_d_b) // 0x50
DUMMY(op_ld_d_c) // 0x51
DUMMY(op_ld_d_d) // 0x52
DUMMY(op_ld_d_e) // 0x53
DUMMY(op_ld_d_h) // 0x54
DUMMY(op_ld_d_l) // 0x55
DUMMY(op_ld_d_hl) // 0x56
DUMMY(op_ld_d_a) // 0x57
DUMMY(op_ld_e_b) // 0x58
DUMMY(op_ld_e_c) // 0x59
DUMMY(op_ld_e_d) // 0x5A
DUMMY(op_ld_e_e) // 0x5B
DUMMY(op_ld_e_h) // 0x5C
DUMMY(op_ld_e_l) // 0x5D
DUMMY(op_ld_e_hl) // 0x5E
DUMMY(op_ld_e_a) // 0x5F