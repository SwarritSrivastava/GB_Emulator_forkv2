#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_bit_0_b) // 0xCB40
DUMMY(op_bit_0_c) // 0xCB41
DUMMY(op_bit_0_d) // 0xCB42
DUMMY(op_bit_0_e) // 0xCB43
DUMMY(op_bit_0_h) // 0xCB44
DUMMY(op_bit_0_l) // 0xCB45
DUMMY(op_bit_0_hl) // 0xCB46
DUMMY(op_bit_0_a) // 0xCB47
DUMMY(op_bit_1_b) // 0xCB48
DUMMY(op_bit_1_c) // 0xCB49
DUMMY(op_bit_1_d) // 0xCB4A
DUMMY(op_bit_1_e) // 0xCB4B
DUMMY(op_bit_1_h) // 0xCB4C
DUMMY(op_bit_1_l) // 0xCB4D
DUMMY(op_bit_1_hl) // 0xCB4E
DUMMY(op_bit_1_a) // 0xCB4F
