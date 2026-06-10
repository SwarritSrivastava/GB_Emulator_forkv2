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
    const u8 newValue = mmu.read(cpu.inc_pc());

    cpu.reg(ProcessingUnit::Register::D) = newValue;

    return totalMachineCycles(2);
}

int op_rla(ProcessingUnit& cpu, MMU& mmu) // 0x17
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const bool carryIn = cpu.get_flag_c();

    const u8 carryOut = (a >> 7) & 1;
    const u8 result = static_cast<u8>((a << 1) | (carryIn ? 1 : 0));

    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, false);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carryOut);

    return totalMachineCycles(1);
}

int op_jr_r8(ProcessingUnit& cpu, MMU& mmu) // 0x18
{
    const int8_t offset = static_cast<int8_t>(mmu.read(cpu.inc_pc()));
    const u16 target = static_cast<u16>(cpu.get_pc() + offset);

    cpu.set_pc(target);

    return totalMachineCycles(3);
}

int op_add_hl_de(ProcessingUnit& cpu, MMU& mmu) // 0x19
{
    const u16 hl = cpu.get_hl();
    const u16 de = cpu.get_de();
    const u32 sum = static_cast<u32>(hl) + de;

    cpu.reg(ProcessingUnit::Register::H) = static_cast<u8>((sum >> 8) & 0xFF);
    cpu.reg(ProcessingUnit::Register::L) = static_cast<u8>(sum & 0xFF);

    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, ((hl & 0x0FFF) + (de & 0x0FFF)) > 0x0FFF);
    cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFFFF);

    return totalMachineCycles(2);
}

int op_ld_a_de(ProcessingUnit& cpu, MMU& mmu) // 0x1A
{
    cpu.reg(ProcessingUnit::Register::A) = mmu.read(cpu.get_de());

    return totalMachineCycles(2);
}

int op_dec_de(ProcessingUnit& cpu, MMU& mmu) // 0x1B
{
    const u16 newValue = cpu.get_de() - 1;

    cpu.reg(ProcessingUnit::Register::D) = static_cast<u8>((newValue >> 8) & 0xFF);
    cpu.reg(ProcessingUnit::Register::E) = static_cast<u8>(newValue & 0xFF);

    return totalMachineCycles(2);
}

int op_inc_e(ProcessingUnit& cpu, MMU& mmu) // 0x1C
{
    const u8 oldValue = cpu.reg(ProcessingUnit::Register::E);
    const u8 newValue = oldValue + 1;
    cpu.reg(ProcessingUnit::Register::E) = newValue;

    cpu.setFlag(ProcessingUnit::Flag::Z, newValue == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, (oldValue & 0x0F) == 0x0F);

    return totalMachineCycles(1);
}

int op_dec_e(ProcessingUnit& cpu, MMU& mmu) // 0x1D
{
    const u8 oldValue = cpu.reg(ProcessingUnit::Register::E);
    const u8 newValue = oldValue - 1;
    cpu.reg(ProcessingUnit::Register::E) = newValue;

    cpu.setFlag(ProcessingUnit::Flag::Z, newValue == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, (oldValue & 0x0F) == 0x00);

    return totalMachineCycles(1);
}

int op_ld_e_d8(ProcessingUnit& cpu, MMU& mmu) // 0x1E
{
    cpu.reg(ProcessingUnit::Register::E) = mmu.read(cpu.inc_pc());

    return totalMachineCycles(2);
}

int op_rra(ProcessingUnit& cpu, MMU& mmu) // 0x1F
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const bool carryIn = cpu.get_flag_c();

    const u8 carryOut = a & 1;
    const u8 result = static_cast<u8>((a >> 1) | (carryIn ? 0x80 : 0x00));

    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, false);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carryOut);

    return totalMachineCycles(1);
}
