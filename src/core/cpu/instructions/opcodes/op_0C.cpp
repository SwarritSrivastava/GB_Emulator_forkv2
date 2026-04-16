#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"
#include "../../../../../include/opcode_table.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_ret_nz(ProcessingUnit& cpu, MMU& mmu) // 0xC0
{
    if (cpu.get_flag_z()) {
        return totalMachineCycles(2);
    }

    const u16 sp = cpu.get_sp();
    const u8 lo = mmu.read(sp);
    const u8 hi = mmu.read(sp + 1);

    cpu.set_sp(sp + 2);
    cpu.set_pc(static_cast<u16>((hi << 8) | lo));

    return totalMachineCycles(5);
}

int op_pop_bc(ProcessingUnit& cpu, MMU& mmu) // 0xC1
{
    const u16 sp = cpu.get_sp();
    const u8 lo = mmu.read(sp);
    const u8 hi = mmu.read(sp + 1);

    cpu.reg(ProcessingUnit::Register::C) = lo;
    cpu.reg(ProcessingUnit::Register::B) = hi;

    cpu.set_sp(sp + 2);
    
    return totalMachineCycles(3);
}

int op_jp_nz(ProcessingUnit& cpu, MMU& mmu) // 0xC2
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());
    const u16 addr = static_cast<u16>((hi << 8) | lo);

    if (!cpu.get_flag_z())
    {
        cpu.set_pc(addr);
        return totalMachineCycles(4);
    }

    return totalMachineCycles(3);
}

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
