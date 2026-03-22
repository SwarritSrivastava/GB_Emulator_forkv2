#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_bit_4_b) // 0xCB60
DUMMY(op_bit_4_c) // 0xCB61
DUMMY(op_bit_4_d) // 0xCB62
DUMMY(op_bit_4_e) // 0xCB63
DUMMY(op_bit_4_h) // 0xCB64
DUMMY(op_bit_4_l) // 0xCB65
DUMMY(op_bit_4_hl) // 0xCB66
DUMMY(op_bit_4_a) // 0xCB67
DUMMY(op_bit_5_b) // 0xCB68
DUMMY(op_bit_5_c) // 0xCB69
DUMMY(op_bit_5_d) // 0xCB6A
DUMMY(op_bit_5_e) // 0xCB6B
DUMMY(op_bit_5_h) // 0xCB6C
DUMMY(op_bit_5_l) // 0xCB6D
DUMMY(op_bit_5_hl) // 0xCB6E
DUMMY(op_bit_5_a) // 0xCB6F
