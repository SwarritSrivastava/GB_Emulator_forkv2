#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_set_6_b) // 0xCBF0
DUMMY(op_set_6_c) // 0xCBF1
DUMMY(op_set_6_d) // 0xCBF2
DUMMY(op_set_6_e) // 0xCBF3
DUMMY(op_set_6_h) // 0xCBF4
DUMMY(op_set_6_l) // 0xCBF5
DUMMY(op_set_6_hl) // 0xCBF6
DUMMY(op_set_6_a) // 0xCBF7
DUMMY(op_set_7_b) // 0xCBF8
DUMMY(op_set_7_c) // 0xCBF9
DUMMY(op_set_7_d) // 0xCBFA
DUMMY(op_set_7_e) // 0xCBFB
DUMMY(op_set_7_h) // 0xCBFC
DUMMY(op_set_7_l) // 0xCBFD
DUMMY(op_set_7_hl) // 0xCBFE
DUMMY(op_set_7_a) // 0xCBFF
