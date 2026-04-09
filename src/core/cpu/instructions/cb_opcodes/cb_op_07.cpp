#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_bit_6_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB70
{
    bool set = (cpu.reg(ProcessingUnit::Register::B) & 0x40) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_6_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB71
{
    bool set = (cpu.reg(ProcessingUnit::Register::C) & 0x40) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_6_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB72
{
    bool set = (cpu.reg(ProcessingUnit::Register::D) & 0x40) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_6_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB73
{
    bool set = (cpu.reg(ProcessingUnit::Register::E) & 0x40) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_6_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB74
{
    bool set = (cpu.reg(ProcessingUnit::Register::H) & 0x40) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_6_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB75
{
    bool set = (cpu.reg(ProcessingUnit::Register::L) & 0x40) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_6_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB76
{
    u8 value = mmu.read(cpu.get_hl());
    bool set = (value & 0x40) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(3);
}

int op_bit_6_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB77
{
    bool set = (cpu.reg(ProcessingUnit::Register::A) & 0x40) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_7_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB78
{
    bool set = (cpu.reg(ProcessingUnit::Register::B) & 0x80) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_7_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB79
{
    bool set = (cpu.reg(ProcessingUnit::Register::C) & 0x80) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_7_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB7A
{
    bool set = (cpu.reg(ProcessingUnit::Register::D) & 0x80) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_7_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB7B
{
    bool set = (cpu.reg(ProcessingUnit::Register::E) & 0x80) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_7_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB7C
{
    bool set = (cpu.reg(ProcessingUnit::Register::H) & 0x80) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_7_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB7D
{
    bool set = (cpu.reg(ProcessingUnit::Register::L) & 0x80) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_7_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB7E
{
    u8 value = mmu.read(cpu.get_hl());
    bool set = (value & 0x80) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(3);
}

int op_bit_7_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB7F
{
    bool set = (cpu.reg(ProcessingUnit::Register::A) & 0x80) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}
