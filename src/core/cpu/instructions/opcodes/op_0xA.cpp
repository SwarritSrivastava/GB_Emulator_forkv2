#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_and_b) // 0xA0
DUMMY(op_and_c) // 0xA1
DUMMY(op_and_d) // 0xA2
DUMMY(op_and_e) // 0xA3
DUMMY(op_and_h) // 0xA4
DUMMY(op_and_l) // 0xA5
DUMMY(op_and_hl) // 0xA6
DUMMY(op_and_a) // 0xA7
DUMMY(op_xor_b) // 0xA8
DUMMY(op_xor_c) // 0xA9
DUMMY(op_xor_d) // 0xAA
DUMMY(op_xor_e) // 0xAB
DUMMY(op_xor_h) // 0xAC
DUMMY(op_xor_l) // 0xAD
DUMMY(op_xor_hl) // 0xAE
DUMMY(op_xor_a) // 0xAF