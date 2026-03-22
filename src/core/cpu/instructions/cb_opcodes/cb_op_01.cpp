#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_rl_b) // 0xCB10
DUMMY(op_rl_c) // 0xCB11
DUMMY(op_rl_d) // 0xCB12
DUMMY(op_rl_e) // 0xCB13
DUMMY(op_rl_h) // 0xCB14
DUMMY(op_rl_l) // 0xCB15
DUMMY(op_rl_hl) // 0xCB16
DUMMY(op_rl_a) // 0xCB17
DUMMY(op_rr_b) // 0xCB18
DUMMY(op_rr_c) // 0xCB19
DUMMY(op_rr_d) // 0xCB1A
DUMMY(op_rr_e) // 0xCB1B
DUMMY(op_rr_h) // 0xCB1C
DUMMY(op_rr_l) // 0xCB1D
DUMMY(op_rr_hl) // 0xCB1E
DUMMY(op_rr_a) // 0xCB1F
