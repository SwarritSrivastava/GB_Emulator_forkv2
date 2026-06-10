#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_jr_nz(ProcessingUnit& cpu, MMU& mmu) // 0x20
{
    const int8_t offset = static_cast<int8_t>(mmu.read(cpu.inc_pc()));

    if (!cpu.get_flag_z()) {
        cpu.set_pc(static_cast<u16>(cpu.get_pc() + offset));
        return totalMachineCycles(3);
    }

    return totalMachineCycles(2);
}

int op_ld_hl_d16(ProcessingUnit& cpu, MMU& mmu) // 0x21
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());

    cpu.reg(ProcessingUnit::Register::H) = hi;
    cpu.reg(ProcessingUnit::Register::L) = lo;

    return totalMachineCycles(3);
}

int op_ld_hli_a(ProcessingUnit& cpu, MMU& mmu) // 0x22
{
    const u16 hl = cpu.get_hl();
    mmu.write(hl, cpu.reg(ProcessingUnit::Register::A));

    const u16 newValue = hl + 1;
    cpu.reg(ProcessingUnit::Register::H) = static_cast<u8>((newValue >> 8) & 0xFF);
    cpu.reg(ProcessingUnit::Register::L) = static_cast<u8>(newValue & 0xFF);

    return totalMachineCycles(2);
}

int op_inc_hl(ProcessingUnit& cpu, MMU& mmu) // 0x23
{
    const u16 newValue = cpu.get_hl() + 1;
    cpu.reg(ProcessingUnit::Register::H) = static_cast<u8>((newValue >> 8) & 0xFF);
    cpu.reg(ProcessingUnit::Register::L) = static_cast<u8>(newValue & 0xFF);

    return totalMachineCycles(2);
}

int op_inc_h(ProcessingUnit& cpu, MMU& mmu) // 0x24
{
    const u8 oldValue = cpu.reg(ProcessingUnit::Register::H);
    const u8 newValue = oldValue + 1;
    cpu.reg(ProcessingUnit::Register::H) = newValue;

    cpu.setFlag(ProcessingUnit::Flag::Z, newValue == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, (oldValue & 0x0F) == 0x0F);

    return totalMachineCycles(1);
}

int op_dec_h(ProcessingUnit& cpu, MMU& mmu) // 0x25
{
    const u8 oldValue = cpu.reg(ProcessingUnit::Register::H);
    const u8 newValue = oldValue - 1;
    cpu.reg(ProcessingUnit::Register::H) = newValue;

    cpu.setFlag(ProcessingUnit::Flag::Z, newValue == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, (oldValue & 0x0F) == 0x00);

    return totalMachineCycles(1);
}

int op_ld_h_d8(ProcessingUnit& cpu, MMU& mmu) // 0x26
{
    cpu.reg(ProcessingUnit::Register::H) = mmu.read(cpu.inc_pc());

    return totalMachineCycles(2);
}

int op_daa(ProcessingUnit& cpu, MMU& mmu) // 0x27
{
    u16 a = cpu.reg(ProcessingUnit::Register::A);
    bool c = cpu.get_flag_c();

    if (!cpu.get_flag_n()) {
        if (cpu.get_flag_h() || (a & 0x0F) > 0x09) {
            a += 0x06;
        }
        if (c || a > 0x9F) {
            a += 0x60;
            c = true;
        }
    } else {
        if (cpu.get_flag_h()) {
            a = (a - 0x06) & 0xFF;
        }
        if (c) {
            a = (a - 0x60) & 0xFF;
        }
    }

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(a & 0xFF);

    cpu.setFlag(ProcessingUnit::Flag::Z, cpu.reg(ProcessingUnit::Register::A) == 0);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, c);

    return totalMachineCycles(1);
}

int op_jr_z(ProcessingUnit& cpu, MMU& mmu) // 0x28
{
    const int8_t offset = static_cast<int8_t>(mmu.read(cpu.inc_pc()));

    if (cpu.get_flag_z()) {
        cpu.set_pc(static_cast<u16>(cpu.get_pc() + offset));
        return totalMachineCycles(3);
    }

    return totalMachineCycles(2);
}

int op_add_hl_hl(ProcessingUnit& cpu, MMU& mmu) // 0x29
{
    const u16 hl = cpu.get_hl();
    const u32 sum = static_cast<u32>(hl) + hl;

    cpu.reg(ProcessingUnit::Register::H) = static_cast<u8>((sum >> 8) & 0xFF);
    cpu.reg(ProcessingUnit::Register::L) = static_cast<u8>(sum & 0xFF);

    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, ((hl & 0x0FFF) + (hl & 0x0FFF)) > 0x0FFF);
    cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFFFF);

    return totalMachineCycles(2);
}

int op_ld_a_hli(ProcessingUnit& cpu, MMU& mmu) // 0x2A
{
    const u16 hl = cpu.get_hl();
    cpu.reg(ProcessingUnit::Register::A) = mmu.read(hl);

    const u16 newValue = hl + 1;
    cpu.reg(ProcessingUnit::Register::H) = static_cast<u8>((newValue >> 8) & 0xFF);
    cpu.reg(ProcessingUnit::Register::L) = static_cast<u8>(newValue & 0xFF);

    return totalMachineCycles(2);
}

int op_dec_hl(ProcessingUnit& cpu, MMU& mmu) // 0x2B
{
    const u16 newValue = cpu.get_hl() - 1;
    cpu.reg(ProcessingUnit::Register::H) = static_cast<u8>((newValue >> 8) & 0xFF);
    cpu.reg(ProcessingUnit::Register::L) = static_cast<u8>(newValue & 0xFF);

    return totalMachineCycles(2);
}

int op_inc_l(ProcessingUnit& cpu, MMU& mmu) // 0x2C
{
    const u8 oldValue = cpu.reg(ProcessingUnit::Register::L);
    const u8 newValue = oldValue + 1;
    cpu.reg(ProcessingUnit::Register::L) = newValue;

    cpu.setFlag(ProcessingUnit::Flag::Z, newValue == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, (oldValue & 0x0F) == 0x0F);

    return totalMachineCycles(1);
}

int op_dec_l(ProcessingUnit& cpu, MMU& mmu) // 0x2D
{
    const u8 oldValue = cpu.reg(ProcessingUnit::Register::L);
    const u8 newValue = oldValue - 1;
    cpu.reg(ProcessingUnit::Register::L) = newValue;

    cpu.setFlag(ProcessingUnit::Flag::Z, newValue == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, (oldValue & 0x0F) == 0x00);

    return totalMachineCycles(1);
}

int op_ld_l_d8(ProcessingUnit& cpu, MMU& mmu) // 0x2E
{
    cpu.reg(ProcessingUnit::Register::L) = mmu.read(cpu.inc_pc());

    return totalMachineCycles(2);
}

int op_cpl(ProcessingUnit& cpu, MMU& mmu) // 0x2F
{
    cpu.reg(ProcessingUnit::Register::A) = ~cpu.reg(ProcessingUnit::Register::A);

    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(1);
}
