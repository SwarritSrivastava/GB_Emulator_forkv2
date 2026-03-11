#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_ldh_a_a8) // 0xF0
DUMMY(op_pop_af) // 0xF1
DUMMY(op_ld_a_c_f2) // 0xF2
DUMMY(op_di) // 0xF3
DUMMY(op_illegal_f4) // 0xF4
DUMMY(op_push_af) // 0xF5
DUMMY(op_or_d8) // 0xF6
DUMMY(op_rst_30) // 0xF7
DUMMY(op_ld_hl_sp_e8) // 0xF8
DUMMY(op_ld_sp_hl) // 0xF9
DUMMY(op_ld_a_a16) // 0xFA
DUMMY(op_ei) // 0xFB
DUMMY(op_illegal_fc) // 0xFC
DUMMY(op_illegal_fd) // 0xFD
DUMMY(op_cp_d8) // 0xFE
DUMMY(op_rst_38) // 0xFF