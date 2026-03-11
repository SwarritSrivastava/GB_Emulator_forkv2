#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_or_b) // 0xB0
DUMMY(op_or_c) // 0xB1
DUMMY(op_or_d) // 0xB2
DUMMY(op_or_e) // 0xB3
DUMMY(op_or_h) // 0xB4
DUMMY(op_or_l) // 0xB5
DUMMY(op_or_hl) // 0xB6
DUMMY(op_or_a) // 0xB7
DUMMY(op_cp_b) // 0xB8
DUMMY(op_cp_c) // 0xB9
DUMMY(op_cp_d) // 0xBA
DUMMY(op_cp_e) // 0xBB
DUMMY(op_cp_h) // 0xBC
DUMMY(op_cp_l) // 0xBD
DUMMY(op_cp_hl) // 0xBE
DUMMY(op_cp_a) // 0xBF