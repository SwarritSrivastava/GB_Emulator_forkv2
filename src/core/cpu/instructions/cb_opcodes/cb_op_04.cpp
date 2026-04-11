#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_bit_0_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB40
{
    bool set = (cpu.reg(ProcessingUnit::Register::B) & 0x01) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_0_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB41
{
    bool set = (cpu.reg(ProcessingUnit::Register::C) & 0x01) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_0_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB42
{
    bool set = (cpu.reg(ProcessingUnit::Register::D) & 0x01) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_0_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB43
{
    bool set = (cpu.reg(ProcessingUnit::Register::E) & 0x01) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_0_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB44
{
    bool set = (cpu.reg(ProcessingUnit::Register::H) & 0x01) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_0_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB45
{
    bool set = (cpu.reg(ProcessingUnit::Register::L) & 0x01) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_0_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB46
{
    u8 value = mmu.read(cpu.get_hl());
    bool set = (value & 0x01) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(3);
}

int op_bit_0_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB47
{
    bool set = (cpu.reg(ProcessingUnit::Register::A) & 0x01) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_1_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB48
{
    bool set = (cpu.reg(ProcessingUnit::Register::B) & 0x02) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_1_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB49
{
    bool set = (cpu.reg(ProcessingUnit::Register::C) & 0x02) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_1_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB4A
{
    bool set = (cpu.reg(ProcessingUnit::Register::D) & 0x02) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_1_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB4B
{
    bool set = (cpu.reg(ProcessingUnit::Register::E) & 0x02) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_1_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB4C
{
    bool set = (cpu.reg(ProcessingUnit::Register::H) & 0x02) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_1_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB4D
{
    bool set = (cpu.reg(ProcessingUnit::Register::L) & 0x02) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}

int op_bit_1_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB4E
{
    u8 value = mmu.read(cpu.get_hl());
    bool set = (value & 0x02) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(3);
}

int op_bit_1_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB4F
{
    bool set = (cpu.reg(ProcessingUnit::Register::A) & 0x02) != 0;

    cpu.setFlag(ProcessingUnit::Flag::Z, !set);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);

    return totalMachineCycles(2);
}
