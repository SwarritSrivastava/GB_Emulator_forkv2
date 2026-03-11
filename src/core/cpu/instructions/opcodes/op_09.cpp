#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_sub_b) // 0x90
DUMMY(op_sub_c) // 0x91
DUMMY(op_sub_d) // 0x92
DUMMY(op_sub_e) // 0x93
DUMMY(op_sub_h) // 0x94
DUMMY(op_sub_l) // 0x95
DUMMY(op_sub_hl) // 0x96
DUMMY(op_sub_a) // 0x97
DUMMY(op_sbc_a_b) // 0x98
DUMMY(op_sbc_a_c) // 0x99
DUMMY(op_sbc_a_d) // 0x9A
DUMMY(op_sbc_a_e) // 0x9B
DUMMY(op_sbc_a_h) // 0x9C
DUMMY(op_sbc_a_l) // 0x9D
DUMMY(op_sbc_a_hl) // 0x9E
DUMMY(op_sbc_a_a) // 0x9F