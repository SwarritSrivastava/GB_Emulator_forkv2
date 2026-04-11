#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_bit_2_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB50
{
    bool set = (cpu.reg(ProcessingUnit::Register::B) & 0x04) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_2_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB51
{
    bool set = (cpu.reg(ProcessingUnit::Register::C) & 0x04) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_2_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB52
{
    bool set = (cpu.reg(ProcessingUnit::Register::D) & 0x04) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_2_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB53
{
    bool set = (cpu.reg(ProcessingUnit::Register::E) & 0x04) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_2_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB54
{
    bool set = (cpu.reg(ProcessingUnit::Register::H) & 0x04) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_2_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB55
{
    bool set = (cpu.reg(ProcessingUnit::Register::L) & 0x04) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_2_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB56
{
    u8 value = mmu.read(cpu.get_hl());
    bool set = (value & 0x04) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(3);
}

int op_bit_2_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB57
{
    bool set = (cpu.reg(ProcessingUnit::Register::A) & 0x04) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_3_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB58
{
    bool set = (cpu.reg(ProcessingUnit::Register::B) & 0x08) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_3_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB59
{
    bool set = (cpu.reg(ProcessingUnit::Register::C) & 0x08) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_3_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB5A
{
    bool set = (cpu.reg(ProcessingUnit::Register::D) & 0x08) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_3_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB5B
{
    bool set = (cpu.reg(ProcessingUnit::Register::E) & 0x08) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_3_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB5C
{
    bool set = (cpu.reg(ProcessingUnit::Register::H) & 0x08) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_3_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB5D
{
    bool set = (cpu.reg(ProcessingUnit::Register::L) & 0x08) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_3_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB5E
{
    u8 value = mmu.read(cpu.get_hl());
    bool set = (value & 0x08) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(3);
}

int op_bit_3_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB5F
{
    bool set = (cpu.reg(ProcessingUnit::Register::A) & 0x08) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}
