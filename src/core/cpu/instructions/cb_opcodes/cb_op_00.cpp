#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_rlc_b) // 0xCB00
DUMMY(op_rlc_c) // 0xCB01
DUMMY(op_rlc_d) // 0xCB02
DUMMY(op_rlc_e) // 0xCB03
DUMMY(op_rlc_h) // 0xCB04
DUMMY(op_rlc_l) // 0xCB05
DUMMY(op_rlc_hl) // 0xCB06
DUMMY(op_rlc_a) // 0xCB07
DUMMY(op_rrc_b) // 0xCB08
DUMMY(op_rrc_c) // 0xCB09
DUMMY(op_rrc_d) // 0xCB0A
DUMMY(op_rrc_e) // 0xCB0B
DUMMY(op_rrc_h) // 0xCB0C
DUMMY(op_rrc_l) // 0xCB0D
DUMMY(op_rrc_hl) // 0xCB0E
DUMMY(op_rrc_a) // 0xCB0F
