#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_res_6_b) // 0xCBB0
DUMMY(op_res_6_c) // 0xCBB1
DUMMY(op_res_6_d) // 0xCBB2
DUMMY(op_res_6_e) // 0xCBB3
DUMMY(op_res_6_h) // 0xCBB4
DUMMY(op_res_6_l) // 0xCBB5
DUMMY(op_res_6_hl) // 0xCBB6
DUMMY(op_res_6_a) // 0xCBB7
DUMMY(op_res_7_b) // 0xCBB8
DUMMY(op_res_7_c) // 0xCBB9
DUMMY(op_res_7_d) // 0xCBBA
DUMMY(op_res_7_e) // 0xCBBB
DUMMY(op_res_7_h) // 0xCBBC
DUMMY(op_res_7_l) // 0xCBBD
DUMMY(op_res_7_hl) // 0xCBBE
DUMMY(op_res_7_a) // 0xCBBF
