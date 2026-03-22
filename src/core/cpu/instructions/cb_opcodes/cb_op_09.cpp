#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_res_2_b) // 0xCB90
DUMMY(op_res_2_c) // 0xCB91
DUMMY(op_res_2_d) // 0xCB92
DUMMY(op_res_2_e) // 0xCB93
DUMMY(op_res_2_h) // 0xCB94
DUMMY(op_res_2_l) // 0xCB95
DUMMY(op_res_2_hl) // 0xCB96
DUMMY(op_res_2_a) // 0xCB97
DUMMY(op_res_3_b) // 0xCB98
DUMMY(op_res_3_c) // 0xCB99
DUMMY(op_res_3_d) // 0xCB9A
DUMMY(op_res_3_e) // 0xCB9B
DUMMY(op_res_3_h) // 0xCB9C
DUMMY(op_res_3_l) // 0xCB9D
DUMMY(op_res_3_hl) // 0xCB9E
DUMMY(op_res_3_a) // 0xCB9F
