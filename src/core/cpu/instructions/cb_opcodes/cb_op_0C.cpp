#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_set_0_b) // 0xCBC0
DUMMY(op_set_0_c) // 0xCBC1
DUMMY(op_set_0_d) // 0xCBC2
DUMMY(op_set_0_e) // 0xCBC3
DUMMY(op_set_0_h) // 0xCBC4
DUMMY(op_set_0_l) // 0xCBC5
DUMMY(op_set_0_hl) // 0xCBC6
DUMMY(op_set_0_a) // 0xCBC7
DUMMY(op_set_1_b) // 0xCBC8
DUMMY(op_set_1_c) // 0xCBC9
DUMMY(op_set_1_d) // 0xCBCA
DUMMY(op_set_1_e) // 0xCBCB
DUMMY(op_set_1_h) // 0xCBCC
DUMMY(op_set_1_l) // 0xCBCD
DUMMY(op_set_1_hl) // 0xCBCE
DUMMY(op_set_1_a) // 0xCBCF
