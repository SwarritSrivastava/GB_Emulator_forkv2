#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_swap_b) // 0xCB30
DUMMY(op_swap_c) // 0xCB31
DUMMY(op_swap_d) // 0xCB32
DUMMY(op_swap_e) // 0xCB33
DUMMY(op_swap_h) // 0xCB34
DUMMY(op_swap_l) // 0xCB35
DUMMY(op_swap_hl) // 0xCB36
DUMMY(op_swap_a) // 0xCB37
DUMMY(op_srl_b) // 0xCB38
DUMMY(op_srl_c) // 0xCB39
DUMMY(op_srl_d) // 0xCB3A
DUMMY(op_srl_e) // 0xCB3B
DUMMY(op_srl_h) // 0xCB3C
DUMMY(op_srl_l) // 0xCB3D
DUMMY(op_srl_hl) // 0xCB3E
DUMMY(op_srl_a) // 0xCB3F
