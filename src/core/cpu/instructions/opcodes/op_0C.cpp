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

int op_jp(ProcessingUnit& cpu, MMU& mmu) // 0xC3
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());

    cpu.set_pc(static_cast<u16>((hi << 8) | lo));

    return totalMachineCycles(4);
}

int op_call_nz(ProcessingUnit& cpu, MMU& mmu) // 0xC4
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());
    const u16 addr = static_cast<u16>((hi << 8) | lo);

    if (!cpu.get_flag_z())
    {
        const u16 pc = cpu.get_pc();
        const u16 sp = cpu.get_sp();

        mmu.write(sp - 1, static_cast<u8>(pc >> 8));
        mmu.write(sp - 2, static_cast<u8>(pc & 0xFF));
        cpu.set_sp(sp - 2);
        cpu.set_pc(addr);

        return totalMachineCycles(6);
    }
    return totalMachineCycles(3);
}

int op_push_bc(ProcessingUnit& cpu, MMU& mmu) // 0xC5
{
    const u8 b = cpu.reg(ProcessingUnit::Register::B);
    const u8 c = cpu.reg(ProcessingUnit::Register::C);
    const u16 sp = cpu.get_sp();

    mmu.write(sp - 1, b);
    mmu.write(sp - 2, c);
    cpu.set_sp(sp - 2);

    return totalMachineCycles(4);
}

int op_add_a_d8(ProcessingUnit& cpu, MMU& mmu) // 0xC6
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 d8 = mmu.read(cpu.inc_pc());
    const u16 result = static_cast<u16>(a) + static_cast<u16>(d8);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (d8 & 0x0F)) > 0x0F);
    cpu.setFlag(ProcessingUnit::Flag::C, result > 0xFF);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);

    return totalMachineCycles(2);
}

int op_rst_00(ProcessingUnit& cpu, MMU& mmu) // 0xC7
{
    const u16 pc = cpu.get_pc();
    const u16 sp = cpu.get_sp();

    mmu.write(sp - 1, static_cast<u8>(pc >> 8));
    mmu.write(sp - 2, static_cast<u8>(pc & 0xFF));
    cpu.set_sp(sp - 2);
    cpu.set_pc(0x0000);

    return totalMachineCycles(4);
}

int op_ret_z(ProcessingUnit& cpu, MMU& mmu) // 0xC8
{
    if (!cpu.get_flag_z()) {
        return totalMachineCycles(2);
    }

    const u16 sp = cpu.get_sp();
    const u8 lo = mmu.read(sp);
    const u8 hi = mmu.read(sp + 1);

    cpu.set_sp(sp + 2);
    cpu.set_pc(static_cast<u16>((hi << 8) | lo));

    return totalMachineCycles(5);
}

int op_ret(ProcessingUnit& cpu, MMU& mmu) // 0xC9
{
    const u16 sp = cpu.get_sp();
    const u8 lo = mmu.read(sp);
    const u8 hi = mmu.read(sp + 1);

    cpu.set_sp(sp + 2);
    cpu.set_pc(static_cast<u16>((hi << 8) | lo));

    return totalMachineCycles(4);
}

int op_jp_z(ProcessingUnit& cpu, MMU& mmu) // 0xCA
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());
    const u16 addr = static_cast<u16>((hi << 8) | lo);

    if (cpu.get_flag_z())
    {
        cpu.set_pc(addr);
        return totalMachineCycles(4);
    }

    return totalMachineCycles(3);
}

int op_cb_prefix(ProcessingUnit& cpu , MMU& mmu) // 0xCB
{
    const u8 cb_opcode = mmu.read(cpu.inc_pc());

    return cbInstructionTable[cb_opcode](cpu, mmu);
}

DUMMY(op_call_z) // 0xCC
DUMMY(op_call) // 0xCD
DUMMY(op_adc_a_d8) // 0xCE
DUMMY(op_rst_08) // 0xCF
