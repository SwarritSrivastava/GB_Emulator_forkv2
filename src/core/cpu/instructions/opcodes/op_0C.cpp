#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"
#include "../../../../../include/opcode_table.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_ret_nz) // 0xC0
DUMMY(op_pop_bc) // 0xC1
DUMMY(op_jp_nz) // 0xC2
DUMMY(op_jp) // 0xC3
DUMMY(op_call_nz) // 0xC4
DUMMY(op_push_bc) // 0xC5
DUMMY(op_add_a_d8) // 0xC6
DUMMY(op_rst_00) // 0xC7
DUMMY(op_ret_z) // 0xC8
DUMMY(op_ret) // 0xC9
DUMMY(op_jp_z) // 0xCA
int op_cb_prefix(ProcessingUnit& cpu , MMU& mmu) // 0xCB
{
    const u8 cb_opcode = mmu.read(cpu.inc_pc());

    return cbInstructionTable[cb_opcode](cpu, mmu);
}
DUMMY(op_call_z) // 0xCC
DUMMY(op_call) // 0xCD
DUMMY(op_adc_a_d8) // 0xCE
DUMMY(op_rst_08) // 0xCF