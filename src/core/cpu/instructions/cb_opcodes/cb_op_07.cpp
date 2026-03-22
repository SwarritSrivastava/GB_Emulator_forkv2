#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_bit_6_b) // 0xCB70
DUMMY(op_bit_6_c) // 0xCB71
DUMMY(op_bit_6_d) // 0xCB72
DUMMY(op_bit_6_e) // 0xCB73
DUMMY(op_bit_6_h) // 0xCB74
DUMMY(op_bit_6_l) // 0xCB75
DUMMY(op_bit_6_hl) // 0xCB76
DUMMY(op_bit_6_a) // 0xCB77
DUMMY(op_bit_7_b) // 0xCB78
DUMMY(op_bit_7_c) // 0xCB79
DUMMY(op_bit_7_d) // 0xCB7A
DUMMY(op_bit_7_e) // 0xCB7B
DUMMY(op_bit_7_h) // 0xCB7C
DUMMY(op_bit_7_l) // 0xCB7D
DUMMY(op_bit_7_hl) // 0xCB7E
DUMMY(op_bit_7_a) // 0xCB7F
