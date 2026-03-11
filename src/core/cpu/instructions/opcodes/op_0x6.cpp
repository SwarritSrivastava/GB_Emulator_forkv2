#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_ld_h_b) // 0x60
DUMMY(op_ld_h_c) // 0x61
DUMMY(op_ld_h_d) // 0x62
DUMMY(op_ld_h_e) // 0x63
DUMMY(op_ld_h_h) // 0x64
DUMMY(op_ld_h_l) // 0x65
DUMMY(op_ld_h_hl) // 0x66
DUMMY(op_ld_h_a) // 0x67
DUMMY(op_ld_l_b) // 0x68
DUMMY(op_ld_l_c) // 0x69
DUMMY(op_ld_l_d) // 0x6A
DUMMY(op_ld_l_e) // 0x6B
DUMMY(op_ld_l_h) // 0x6C
DUMMY(op_ld_l_l) // 0x6D
DUMMY(op_ld_l_hl) // 0x6E
DUMMY(op_ld_l_a) // 0x6F