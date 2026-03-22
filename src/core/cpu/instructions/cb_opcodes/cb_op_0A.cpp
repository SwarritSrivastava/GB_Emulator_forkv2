#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_res_4_b) // 0xCBA0
DUMMY(op_res_4_c) // 0xCBA1
DUMMY(op_res_4_d) // 0xCBA2
DUMMY(op_res_4_e) // 0xCBA3
DUMMY(op_res_4_h) // 0xCBA4
DUMMY(op_res_4_l) // 0xCBA5
DUMMY(op_res_4_hl) // 0xCBA6
DUMMY(op_res_4_a) // 0xCBA7
DUMMY(op_res_5_b) // 0xCBA8
DUMMY(op_res_5_c) // 0xCBA9
DUMMY(op_res_5_d) // 0xCBAA
DUMMY(op_res_5_e) // 0xCBAB
DUMMY(op_res_5_h) // 0xCBAC
DUMMY(op_res_5_l) // 0xCBAD
DUMMY(op_res_5_hl) // 0xCBAE
DUMMY(op_res_5_a) // 0xCBAF
