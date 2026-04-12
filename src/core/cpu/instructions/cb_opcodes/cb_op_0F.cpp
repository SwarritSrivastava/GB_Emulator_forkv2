#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_set_6_b(ProcessingUnit& cpu, MMU& mmu) // 0xCBF0
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);

    u8 result = value | 0x40;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_set_6_c(ProcessingUnit& cpu, MMU& mmu) // 0xCBF1
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);

    u8 result = value | 0x40;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_set_6_d(ProcessingUnit& cpu, MMU& mmu) // 0xCBF2
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);

    u8 result = value | 0x40;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_set_6_e(ProcessingUnit& cpu, MMU& mmu) // 0xCBF3
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);

    u8 result = value | 0x40;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_set_6_h(ProcessingUnit& cpu, MMU& mmu) // 0xCBF4
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);

    u8 result = value | 0x40;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_set_6_l(ProcessingUnit& cpu, MMU& mmu) // 0xCBF5
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);

    u8 result = value | 0x40;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_set_6_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCBF6
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);

    u8 result = value | 0x40;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_set_6_a(ProcessingUnit& cpu, MMU& mmu) // 0xCBF7
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);

    u8 result = value | 0x40;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}

int op_set_7_b(ProcessingUnit& cpu, MMU& mmu) // 0xCBF8
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);

    u8 result = value | 0x80;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_set_7_c(ProcessingUnit& cpu, MMU& mmu) // 0xCBF9
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);

    u8 result = value | 0x80;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_set_7_d(ProcessingUnit& cpu, MMU& mmu) // 0xCBFA
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);

    u8 result = value | 0x80;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_set_7_e(ProcessingUnit& cpu, MMU& mmu) // 0xCBFB
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);

    u8 result = value | 0x80;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_set_7_h(ProcessingUnit& cpu, MMU& mmu) // 0xCBFC
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);

    u8 result = value | 0x80;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_set_7_l(ProcessingUnit& cpu, MMU& mmu) // 0xCBFD
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);

    u8 result = value | 0x80;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_set_7_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCBFE
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);

    u8 result = value | 0x80;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_set_7_a(ProcessingUnit& cpu, MMU& mmu) // 0xCBFF
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);

    u8 result = value | 0x80;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}
