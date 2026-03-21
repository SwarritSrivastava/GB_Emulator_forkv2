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

int op_ld_de_d16(ProcessingUnit &cpu, MMU &mmu) // 0x11
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());

    cpu.reg(ProcessingUnit::Register::D) = hi;
    cpu.reg(ProcessingUnit::Register::E) = lo;

    return totalMachineCycles(3);
}

int op_ld_de_a(ProcessingUnit &cpu, MMU &mmu) // 0x12
{
    const u16 addr = cpu.get_de();      // DE contains the target address
    const u8 value = cpu.reg(ProcessingUnit::Register::A);

    mmu.write(addr, value);

    return totalMachineCycles(2);
}

int op_inc_de(ProcessingUnit& cpu, MMU& mmu) // 0x13
{
    const u16 newValue = cpu.get_de() + 1;

    cpu.reg(ProcessingUnit::Register::D) = (newValue >> 8) & 0xFF;
    cpu.reg(ProcessingUnit::Register::E) = newValue & 0xFF;

    return totalMachineCycles(2);
}

int op_inc_d(ProcessingUnit& cpu, MMU& mmu) // 0x14
{
    const u8 oldValue = cpu.reg(ProcessingUnit::Register::D);
    const u8 newValue = oldValue + 1;
    cpu.reg(ProcessingUnit::Register::D) = newValue;
    cpu.setFlag(ProcessingUnit::Flag::Z, newValue == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, (oldValue & 0x0F) == 0x0F);
    return totalMachineCycles(1);
}

int op_dec_d(ProcessingUnit& cpu, MMU& mmu) // 0x15
{
    const u8 oldValue = cpu.reg(ProcessingUnit::Register::D);
    const u8 newValue = oldValue - 1;
    cpu.reg(ProcessingUnit::Register::D) = newValue;
    cpu.setFlag(ProcessingUnit::Flag::Z, newValue == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, (oldValue & 0x0F) == 0x00);
    return totalMachineCycles(1);
}

int op_ld_d_d8(ProcessingUnit& cpu, MMU& mmu) // 0x16
{
    cpu.inc_pc();
    const u8 newValue = mmu.read(cpu.inc_pc());

    cpu.reg(ProcessingUnit::Register::D) = newValue;

    return totalMachineCycles(2);
}

DUMMY(op_rla) // 0x17
DUMMY(op_jr_r8) // 0x18
DUMMY(op_add_hl_de) // 0x19
DUMMY(op_ld_a_de) // 0x1A
DUMMY(op_dec_de) // 0x1B
DUMMY(op_inc_e) // 0x1C
DUMMY(op_dec_e) // 0x1D
DUMMY(op_ld_e_d8) // 0x1E
DUMMY(op_rra) // 0x1F
