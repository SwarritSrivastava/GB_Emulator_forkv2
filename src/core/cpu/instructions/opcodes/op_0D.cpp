#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_ret_nc) // 0xD0
DUMMY(op_pop_de) // 0xD1
DUMMY(op_jp_nc) // 0xD2
DUMMY(op_illegal_d3) // 0xD3
DUMMY(op_call_nc) // 0xD4
DUMMY(op_push_de) // 0xD5
DUMMY(op_sub_d8) // 0xD6
DUMMY(op_rst_10) // 0xD7
DUMMY(op_ret_c) // 0xD8
DUMMY(op_reti) // 0xD9
DUMMY(op_jp_c) // 0xDA
DUMMY(op_illegal_db) // 0xDB
DUMMY(op_call_c) // 0xDC
DUMMY(op_illegal_dd) // 0xDD
DUMMY(op_sbc_a_d8) // 0xDE
DUMMY(op_rst_18) // 0xDF