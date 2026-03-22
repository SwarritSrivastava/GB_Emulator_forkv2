#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_set_2_b) // 0xCBD0
DUMMY(op_set_2_c) // 0xCBD1
DUMMY(op_set_2_d) // 0xCBD2
DUMMY(op_set_2_e) // 0xCBD3
DUMMY(op_set_2_h) // 0xCBD4
DUMMY(op_set_2_l) // 0xCBD5
DUMMY(op_set_2_hl) // 0xCBD6
DUMMY(op_set_2_a) // 0xCBD7
DUMMY(op_set_3_b) // 0xCBD8
DUMMY(op_set_3_c) // 0xCBD9
DUMMY(op_set_3_d) // 0xCBDA
DUMMY(op_set_3_e) // 0xCBDB
DUMMY(op_set_3_h) // 0xCBDC
DUMMY(op_set_3_l) // 0xCBDD
DUMMY(op_set_3_hl) // 0xCBDE
DUMMY(op_set_3_a) // 0xCBDF
