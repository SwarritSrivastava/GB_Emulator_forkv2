#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_res_0_b) // 0xCB80
DUMMY(op_res_0_c) // 0xCB81
DUMMY(op_res_0_d) // 0xCB82
DUMMY(op_res_0_e) // 0xCB83
DUMMY(op_res_0_h) // 0xCB84
DUMMY(op_res_0_l) // 0xCB85
DUMMY(op_res_0_hl) // 0xCB86
DUMMY(op_res_0_a) // 0xCB87
DUMMY(op_res_1_b) // 0xCB88
DUMMY(op_res_1_c) // 0xCB89
DUMMY(op_res_1_d) // 0xCB8A
DUMMY(op_res_1_e) // 0xCB8B
DUMMY(op_res_1_h) // 0xCB8C
DUMMY(op_res_1_l) // 0xCB8D
DUMMY(op_res_1_hl) // 0xCB8E
DUMMY(op_res_1_a) // 0xCB8F
