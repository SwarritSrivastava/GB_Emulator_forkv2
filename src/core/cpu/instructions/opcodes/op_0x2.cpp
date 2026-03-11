#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_jr_nz) // 0x20
DUMMY(op_ld_hl_d16) // 0x21
DUMMY(op_ld_hli_a) // 0x22
DUMMY(op_inc_hl) // 0x23
DUMMY(op_inc_h) // 0x24
DUMMY(op_dec_h) // 0x25
DUMMY(op_ld_h_d8) // 0x26
DUMMY(op_daa) // 0x27
DUMMY(op_jr_z) // 0x28
DUMMY(op_add_hl_hl) // 0x29
DUMMY(op_ld_a_hli) // 0x2A
DUMMY(op_dec_hl) // 0x2B
DUMMY(op_inc_l) // 0x2C
DUMMY(op_dec_l) // 0x2D
DUMMY(op_ld_l_d8) // 0x2E
DUMMY(op_cpl) // 0x2F