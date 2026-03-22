#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_bit_2_b) // 0xCB50
DUMMY(op_bit_2_c) // 0xCB51
DUMMY(op_bit_2_d) // 0xCB52
DUMMY(op_bit_2_e) // 0xCB53
DUMMY(op_bit_2_h) // 0xCB54
DUMMY(op_bit_2_l) // 0xCB55
DUMMY(op_bit_2_hl) // 0xCB56
DUMMY(op_bit_2_a) // 0xCB57
DUMMY(op_bit_3_b) // 0xCB58
DUMMY(op_bit_3_c) // 0xCB59
DUMMY(op_bit_3_d) // 0xCB5A
DUMMY(op_bit_3_e) // 0xCB5B
DUMMY(op_bit_3_h) // 0xCB5C
DUMMY(op_bit_3_l) // 0xCB5D
DUMMY(op_bit_3_hl) // 0xCB5E
DUMMY(op_bit_3_a) // 0xCB5F
