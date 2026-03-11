#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_jr_nc) // 0x30
DUMMY(op_ld_sp_d16) // 0x31
DUMMY(op_ld_hld_a) // 0x32
DUMMY(op_inc_sp) // 0x33
DUMMY(op_inc_hl_ptr) // 0x34
DUMMY(op_dec_hl_ptr) // 0x35
DUMMY(op_ld_hl_d8) // 0x36
DUMMY(op_scf) // 0x37
DUMMY(op_jr_c) // 0x38
DUMMY(op_add_hl_sp) // 0x39
DUMMY(op_ld_a_hld) // 0x3A
DUMMY(op_dec_sp) // 0x3B
DUMMY(op_inc_a) // 0x3C
DUMMY(op_dec_a) // 0x3D
DUMMY(op_ld_a_d8) // 0x3E
DUMMY(op_ccf) // 0x3F