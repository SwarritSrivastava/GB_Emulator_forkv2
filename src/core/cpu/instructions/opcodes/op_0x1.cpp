#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_stop(ProcessingUnit& cpu, MMU& mmu) // 0x10
{
    cpu.setStop(true);
    return totalMachineCycles(1);
}

DUMMY(op_ld_de_d16) // 0x11
DUMMY(op_ld_de_a) // 0x12
DUMMY(op_inc_de) // 0x13
DUMMY(op_inc_d) // 0x14
DUMMY(op_dec_d) // 0x15
DUMMY(op_ld_d_d8) // 0x16
DUMMY(op_rla) // 0x17
DUMMY(op_jr_r8) // 0x18
DUMMY(op_add_hl_de) // 0x19
DUMMY(op_ld_a_de) // 0x1A
DUMMY(op_dec_de) // 0x1B
DUMMY(op_inc_e) // 0x1C
DUMMY(op_dec_e) // 0x1D
DUMMY(op_ld_e_d8) // 0x1E
DUMMY(op_rra) // 0x1F