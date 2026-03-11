#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_ldh_a8_a) // 0xE0
DUMMY(op_pop_hl) // 0xE1
DUMMY(op_ld_c_a_e2) // 0xE2
DUMMY(op_illegal_e3) // 0xE3
DUMMY(op_illegal_e4) // 0xE4
DUMMY(op_push_hl) // 0xE5
DUMMY(op_and_d8) // 0xE6
DUMMY(op_rst_20) // 0xE7
DUMMY(op_add_sp_e8) // 0xE8
DUMMY(op_jp_hl) // 0xE9
DUMMY(op_ld_a16_a) // 0xEA
DUMMY(op_illegal_eb) // 0xEB
DUMMY(op_illegal_ec) // 0xEC
DUMMY(op_illegal_ed) // 0xED
DUMMY(op_xor_d8) // 0xEE
DUMMY(op_rst_28) // 0xEF