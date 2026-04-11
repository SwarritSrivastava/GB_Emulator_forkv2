#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_bit_4_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB60
{
    bool set = (cpu.reg(ProcessingUnit::Register::B) & 0x10) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_4_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB61
{
    bool set = (cpu.reg(ProcessingUnit::Register::C) & 0x10) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_4_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB62
{
    bool set = (cpu.reg(ProcessingUnit::Register::D) & 0x10) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_4_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB63
{
    bool set = (cpu.reg(ProcessingUnit::Register::E) & 0x10) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_4_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB64
{
    bool set = (cpu.reg(ProcessingUnit::Register::H) & 0x10) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_4_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB65
{
    bool set = (cpu.reg(ProcessingUnit::Register::L) & 0x10) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_4_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB66
{
    u8 value = mmu.read(cpu.get_hl());
    bool set = (value & 0x10) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(3);
}

int op_bit_4_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB67
{
    bool set = (cpu.reg(ProcessingUnit::Register::A) & 0x10) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_5_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB68
{
    bool set = (cpu.reg(ProcessingUnit::Register::B) & 0x20) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_5_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB69
{
    bool set = (cpu.reg(ProcessingUnit::Register::C) & 0x20) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_5_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB6A
{
    bool set = (cpu.reg(ProcessingUnit::Register::D) & 0x20) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_5_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB6B
{
    bool set = (cpu.reg(ProcessingUnit::Register::E) & 0x20) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_5_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB6C
{
    bool set = (cpu.reg(ProcessingUnit::Register::H) & 0x20) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_5_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB6D
{
    bool set = (cpu.reg(ProcessingUnit::Register::L) & 0x20) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_5_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB6E
{
    u8 value = mmu.read(cpu.get_hl());
    bool set = (value & 0x20) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(3);
}

int op_bit_5_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB6F
{
    bool set = (cpu.reg(ProcessingUnit::Register::A) & 0x20) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}
