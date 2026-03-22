#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_set_4_b) // 0xCBE0
DUMMY(op_set_4_c) // 0xCBE1
DUMMY(op_set_4_d) // 0xCBE2
DUMMY(op_set_4_e) // 0xCBE3
DUMMY(op_set_4_h) // 0xCBE4
DUMMY(op_set_4_l) // 0xCBE5
DUMMY(op_set_4_hl) // 0xCBE6
DUMMY(op_set_4_a) // 0xCBE7
DUMMY(op_set_5_b) // 0xCBE8
DUMMY(op_set_5_c) // 0xCBE9
DUMMY(op_set_5_d) // 0xCBEA
DUMMY(op_set_5_e) // 0xCBEB
DUMMY(op_set_5_h) // 0xCBEC
DUMMY(op_set_5_l) // 0xCBED
DUMMY(op_set_5_hl) // 0xCBEE
DUMMY(op_set_5_a) // 0xCBEF
